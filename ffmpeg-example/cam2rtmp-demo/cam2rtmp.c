#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>  
#include <libavutil/time.h> 
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavutil/error.h>

#include <pthread.h>

#define INBUF_SIZE 4096

struct sDecoder{
    char *filename;
    const AVCodec *codec;
    AVCodecParserContext *parser;
    AVFormatContext *ifmt_ctx;
    AVCodecContext *c;
    AVFrame *frame;
    AVPacket *pkt;
    FILE *infile;
    int videoindex, audioindex;
}sDecoder;

struct sEncoder{
    char *filename;
    AVFormatContext *ofmt_ctx; 
    const AVCodec *codec;
    AVCodecContext *c;
    uint8_t endcode[4];
    AVPacket *pkt;
    AVFrame *frame;
    uint8_t *data_buffer;
    FILE *outfile;
	struct SwsContext *convert_ctx;
    AVStream *out_stream;
    int64_t start_time;  
}sEncoder;

struct struct_for_thread{
    struct sDecoder *d;
    struct sEncoder *e;
    int decode;
    int encode;
    int run;
}thread_args;

static int64_t start_time, now_time, lasttime, cnt=0;  
void push_stream(AVPacket *pkt, AVCodecContext *in_stream, AVStream *out_stream, AVFormatContext *ofmt_ctx)
{
    //转换PTS/DTS（Convert PTS/DTS）  
    // pkt->pts = av_rescale_q_rnd(pkt->pts, in_stream->time_base, out_stream->time_base, 
    //                             (AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));  
    // pkt->dts = av_rescale_q_rnd(pkt->dts, in_stream->time_base, out_stream->time_base,
    //                             (AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));  
    // pkt->duration = av_rescale_q(pkt->duration, in_stream->time_base, out_stream->time_base);

    // pkt->pos = -1;
    
    printf("pts:%d dts:%d duration:%d\n", pkt->pts, pkt->dts, pkt->duration);
    int ret = av_write_frame(ofmt_ctx, pkt);
    if (ret < 0) {
        printf( "Error muxing packet\n");
        exit(1);
    }
    now_time = av_gettime();
    if (!cnt) {
        start_time = now_time;
    }
    cnt ++;
    lasttime = now_time - start_time + 1;

    printf("--------FPS:%ld---------\n", cnt * 1000000 / lasttime);

}
void *encode_thread(void *args)
{
    int ret;

    while (thread_args.run)
    {
        while(!thread_args.encode);
        ret = avcodec_receive_packet(thread_args.e->c, thread_args.e->pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            continue;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }

        printf("Write packet %3"PRId64" (size=%5d)\n", thread_args.e->pkt->pts, thread_args.e->pkt->size);

        if (thread_args.e->ofmt_ctx && !(thread_args.e->ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
            push_stream(thread_args.e->pkt, thread_args.e->c, thread_args.e->out_stream, thread_args.e->ofmt_ctx);
        } else {
            fwrite(thread_args.e->pkt->data, 1, thread_args.e->pkt->size, thread_args.e->outfile);
        }
        av_packet_unref(thread_args.e->pkt);

        thread_args.encode = 0;
    }
}


void *decode_thread(void *args)
{
    int ret;

    while (thread_args.run)
    {
        while(!thread_args.decode);
        ret = avcodec_receive_frame(thread_args.d->c, thread_args.d->frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            continue;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        printf("saving frame %3d\n", thread_args.d->c->frame_number);
        sws_scale(thread_args.e->convert_ctx, (const uint8_t* const*)thread_args.d->frame->data, thread_args.d->frame->linesize, \
                0, thread_args.e->c->height, thread_args.e->frame->data, thread_args.e->frame->linesize);
        thread_args.e->frame->pts = thread_args.d->c->frame_number;
        /* send the frame to the encoder */
        if (thread_args.e->frame)
            printf("Send frame %3"PRId64"\n", thread_args.e->frame->pts);

        ret = avcodec_send_frame(thread_args.e->c, thread_args.e->frame);
        if (ret < 0) {
            fprintf(stderr, "Error sending a frame for encoding\n");
            exit(1);
        }

        thread_args.decode = 0;
        thread_args.encode = 1;
    }
}

int decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt, struct sEncoder *e)
{
    char buf[1024];
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {

    } else if(ret < 0){
        fprintf(stderr, "Error sending a packet for decoding: %s\n", av_err2str(ret));
        exit(1);
    }

    thread_args.decode = 1;
    return 0;
}
int encode_start(struct sEncoder* e, int width, int height)
{
    //使用H264编码的时候 实时直播延时有3s，改成AV_CODEC_ID_FLV1编码之后几乎感觉不到延时
	e->codec = avcodec_find_encoder(AV_CODEC_ID_FLV1);
	//e->codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    /* find the mpeg1video encoder */
    //codec = avcodec_find_encoder_by_name(codec_name);
    if (!e->codec) {
        fprintf(stderr, "Codec 'H264' not found\n");
        exit(1);
    }

    e->pkt = av_packet_alloc();
    if (!e->pkt)
        exit(1);

    e->c = avcodec_alloc_context3(e->codec);
    if (!e->c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* put sample parameters */
    e->c->bit_rate = 4000000;
    /* resolution must be a multiple of two */
    e->c->width = width;
    e->c->height = height;
    printf("\n--------%dx%d----------\n",width,height);
    /* frames per second */
    e->c->time_base = (AVRational){1, 25};
    e->c->framerate = (AVRational){25, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    e->c->gop_size = 5;
    e->c->max_b_frames = 0;
    e->c->pix_fmt = AV_PIX_FMT_YUV420P;
    e->c->codec_type = AVMEDIA_TYPE_VIDEO;

    if (e->codec->id == AV_CODEC_ID_H264) {
        av_opt_set(e->c->priv_data, "preset", "slow", 0);
        av_opt_set(e->c->priv_data,"tune","zerolatency",0); //加上该参数之后直播延时从10s减少到3s.
    }
    /* open it */
    int ret = avcodec_open2(e->c, e->codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }

    e->frame = av_frame_alloc();
    if (!e->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    e->frame->format = e->c->pix_fmt;
    e->frame->width  = e->c->width;
    e->frame->height = e->c->height;

    ret = av_frame_get_buffer(e->frame, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }

    if (!strncmp("rtmp://", e->filename, 7)) {
        avformat_alloc_output_context2(&e->ofmt_ctx, NULL, "flv", NULL); //RTMP  
        //avformat_alloc_output_context2(&ofmt_ctx, NULL, "rtp", NULL);// RTP/RTCP于UDP传输，VLC播放需要SDP文件
        if (!e->ofmt_ctx) {
            printf( "Could not create output context\n");
            exit(1);
        }
        printf("ofmt_ctx->filename: %s\n",e->ofmt_ctx->filename );
        e->out_stream = avformat_new_stream(e->ofmt_ctx, e->codec);
        if (!e->out_stream) {
            printf( "Could not create new output stream.\n");
            exit(1);
        }
        //avcodec_copy_context(e->out_stream->codecpar,e->c);
        avcodec_parameters_from_context(e->out_stream->codecpar, e->c);
        //e->out_stream->codec->codec_tag = 0;
        //if (e->ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
        //    e->out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;

        //Dump Format------------------
        av_dump_format(e->ofmt_ctx, 0, e->filename, 1);

        //打开输出URL（Open output URL）
        if (!(e->ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
            ret = avio_open(&e->ofmt_ctx->pb, e->filename, AVIO_FLAG_WRITE);
            if (ret < 0) {
                printf( "Could not open output URL '%s'", e->filename);
                exit(1);
            }
        }

        //写文件头（Write file header）
        ret= avformat_write_header(e->ofmt_ctx, NULL);
        if (ret < 0) {
            printf( "Error occurred when opening output URL\n");
            exit(1);
        }
        e->start_time = av_gettime();
    } else { 
        e->outfile = fopen(e->filename, "wb");
        if (!e->outfile) {
            fprintf(stderr, "Could not open %s\n", e->filename);
            exit(1);
        }
    }
    return 0; 
}

void encode_finish(struct sEncoder e)
{
    /* close output */  
    if (e.ofmt_ctx && !(e.ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        avio_close(e.ofmt_ctx->pb);
        avformat_free_context(e.ofmt_ctx);
    } else {
        uint8_t endcode[] = { 0, 0, 1, 0xb7 };
        fwrite(endcode, 1, sizeof(endcode), e.outfile);
        fclose(e.outfile);
    }
	sws_freeContext(e.convert_ctx);
    avcodec_free_context(&e.c);
    av_frame_free(&e.frame);
    av_packet_free(&e.pkt);
}
int decode_start(struct sDecoder *d)
{
    int ret, i;
    AVInputFormat *ifmt = NULL;
    AVDictionary *options = NULL;

    if (!strncmp("/dev/video", d->filename, 10)) {
        printf("---------videoindex---------\n");
        ifmt = av_find_input_format("video4linux2");
        //set width*height
        //av_dict_set(&options, "video_size", "640x480", 0);
        av_dict_set(&options, "framerate", "30", 0);
        // pixel_format == input_format
        //av_dict_set(&options, "pixel_format", "mjpeg", 0); //mjpeg 格式时，在摄像头内进行了数据压缩，所以帧率高，但是delay大
        av_dict_set(&options, "input_format", "yuyv422", 0); //yuv格式帧率低，但是delay小

    }

    //输入（Input）
    if ((ret = avformat_open_input(&d->ifmt_ctx, d->filename, ifmt, &options)) < 0) {
        printf( "Could not open input file:%s[%s].\n", d->filename, av_err2str(ret));
        exit(1);
    }

    if ((ret = avformat_find_stream_info(d->ifmt_ctx, 0)) < 0) {
        printf( "Failed to retrieve input stream information");
        exit(1);
    }

    d->videoindex = av_find_best_stream(d->ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    /*
    d->videoindex = -1;
    for(i=0; i < d->ifmt_ctx->nb_streams; i++){
        if(d->ifmt_ctx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_AUDIO){
            d->audioindex=i;
        }

        if(d->ifmt_ctx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO){
            d->videoindex = i;
            break;
        }
    }*/

    if (d->videoindex == -1) {
		printf("Didn't find a video stream.\n");
        exit(1);
    }

	d->c = avcodec_alloc_context3(NULL);
	if (d->c == NULL)
	{
		printf("Could not allocate AVCodecContext\n");
        exit(1);
	}

	avcodec_parameters_to_context(d->c, d->ifmt_ctx->streams[d->videoindex]->codecpar);
    d->codec = avcodec_find_decoder(d->c->codec_id);
    if (!d->codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    /* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */

    /* open it */
    if (avcodec_open2(d->c, d->codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }
    //获取帧率信息，注意输入流里面用r_frame_rate不用avg_frame_rate
    double dInputFps = d->ifmt_ctx->streams[d->videoindex]->r_frame_rate.num * 1.0 / d->ifmt_ctx->streams[d->videoindex]->r_frame_rate.den;
    printf("camera framerate: %.1f\n", dInputFps);
    av_dump_format(d->ifmt_ctx, 0, d->filename, 0);
    //--------decoder--------------
    d->pkt = av_packet_alloc();
    if (!d->pkt)
        exit(1);

    d->frame = av_frame_alloc();
    if (!d->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    return 0;
}

void decode_finish(struct sDecoder d)
{
	avcodec_close(d.c);
	avformat_close_input(&d.ifmt_ctx);
    av_frame_free(&d.frame);
    av_packet_free(&d.pkt);
}


int main(int argc, char **argv)
{
    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(0);
    }
    
    memset((uint8_t*)&sDecoder, 0, sizeof(sDecoder));
    memset((uint8_t*)&sEncoder, 0, sizeof(sEncoder));

    sDecoder.filename = argv[1];
    sEncoder.filename = argv[2];

    av_register_all();
    avformat_network_init();
    avdevice_register_all();

    decode_start(&sDecoder);

    encode_start(&sEncoder, sDecoder.c->width, sDecoder.c->height);

    /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */

    //解码数据转成RGB24
	//sEncoder.convert_ctx = sws_getContext(sDecoder.c->width, sDecoder.c->height, sDecoder.c->pix_fmt, sDecoder.c->width, sDecoder.c->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
	sEncoder.convert_ctx = sws_getContext(sDecoder.c->width, sDecoder.c->height, sDecoder.c->pix_fmt, sDecoder.c->width, sDecoder.c->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    int i = 0;
    thread_args.d = &sDecoder;
    thread_args.e = &sEncoder;
    thread_args.decode = 0;
    thread_args.encode = 0;
    thread_args.run = 1;

    pthread_t th1, th2;

    pthread_create(&th1, NULL, decode_thread, NULL);
    pthread_create(&th2, NULL, encode_thread, NULL);

    while(av_read_frame(sDecoder.ifmt_ctx,sDecoder.pkt)>=0){
		if ( sDecoder.pkt->stream_index == sDecoder.videoindex ) {
            decode(sDecoder.c, sDecoder.frame, sDecoder.pkt, &sEncoder);
        }
        av_packet_unref(sDecoder.pkt);
        //av_usleep(1000000/25);  
	}

    /* flush the decoder */
    decode(sDecoder.c, sDecoder.frame, NULL, &sEncoder);

    thread_args.run = 0;
    encode_finish(sEncoder);
    decode_finish(sDecoder);
    pthread_join(th2, NULL);
    pthread_join(th1, NULL);


    return 0;
}
