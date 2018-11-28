/*
 * @file: rgb2h264.c
 * @create time: 2018-10-25 16:28:35
 * @last modified: 2018-10-25 16:28:35
 * @description:
 */
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
 
 
static AVCodecContext *c = NULL;
static AVFrame *frame;
static AVPacket pkt;
static FILE *file;
struct SwsContext *sws_context = NULL;
 
static void ffmpeg_encoder_set_frame_yuv_from_rgb(uint8_t *rgb) {
	const int in_linesize[1] = { 3 * c->width };
	sws_context = sws_getCachedContext(sws_context,
		c->width, c->height, AV_PIX_FMT_RGB24,
		c->width, c->height, AV_PIX_FMT_YUV420P,
		0, 0, 0, 0);
	sws_scale(sws_context, (const uint8_t * const *)&rgb, in_linesize, 0,
		c->height, frame->data, frame->linesize);
}
 
uint8_t* generate_rgb(int width, int height, int pts, uint8_t *rgb) {
	int x, y, cur;
	rgb = (uint8_t *)realloc(rgb, 3 * sizeof(uint8_t) * height * width);
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			cur = 3 * (y * width + x);
			rgb[cur + 0] = 0;
			rgb[cur + 1] = 0;
			rgb[cur + 2] = 0;
			if ((frame->pts / 25) % 2 == 0) {
				if (y < height / 2) {
					if (x < width / 2) {
						/* Black. */
					}
					else {
						rgb[cur + 0] = 255;
					}
				}
				else {
					if (x < width / 2) {
						rgb[cur + 1] = 255;
					}
					else {
						rgb[cur + 2] = 255;
					}
				}
			}
			else {
				if (y < height / 2) {
					rgb[cur + 0] = 255;
					if (x < width / 2) {
						rgb[cur + 1] = 255;
					}
					else {
						rgb[cur + 2] = 255;
					}
				}
				else {
					if (x < width / 2) {
						rgb[cur + 1] = 255;
						rgb[cur + 2] = 255;
					}
					else {
						rgb[cur + 0] = 255;
						rgb[cur + 1] = 255;
						rgb[cur + 2] = 255;
					}
				}
			}
		}
	}
	return rgb;
}
 
/* Allocate resources and write header data to the output file. */
void ffmpeg_encoder_start(const char *filename, int codec_id, int fps, int width, int height) {
	AVCodec *codec;
	int ret;
 
	codec = avcodec_find_encoder((enum AVCodecID)codec_id);
	if (!codec) {
		fprintf(stderr, "Codec not found\n");
		exit(1);
	}
	c = avcodec_alloc_context3(codec);
	if (!c) {
		fprintf(stderr, "Could not allocate video codec context\n");
		exit(1);
	}
	c->bit_rate = 400000;
	c->width = width;
	c->height = height;
	c->time_base.num = 1;
	c->time_base.den = fps;
	c->keyint_min = 600;
	c->pix_fmt = AV_PIX_FMT_YUV420P;
	if (codec_id == AV_CODEC_ID_H264)
		av_opt_set(c->priv_data, "preset", "slow", 0);
	if (avcodec_open2(c, codec, NULL) < 0) {
		fprintf(stderr, "Could not open codec\n");
		exit(1);
	}
	file = fopen(filename, "wb");
	if (!file) {
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}
	frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}
	frame->format = c->pix_fmt;
	frame->width = c->width;
	frame->height = c->height;
    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }
	av_init_packet(&pkt);
	pkt.data = NULL;
	pkt.size = 0;
}
int encode(AVCodecContext *enc_ctx, AVPacket *pkt, AVFrame *frame, int *over)
{
    int ret;

    *over = 0;
    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3"PRId64"\n", frame->pts);

    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        return ret;
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return 1;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            return ret;
        }

        printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
        //fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
        *over = 1;
    }
    return 0;
}


/*
Write trailing data to the output file
and free resources allocated by ffmpeg_encoder_start.
*/
void ffmpeg_encoder_finish(void) {
	uint8_t endcode[] = { 0, 0, 1, 0xb7 };
	int got_output, ret;
    
    ret = encode(c, &pkt, NULL, &got_output);
    if (got_output) {
        fwrite(pkt.data, 1, pkt.size, file);
        av_packet_unref(&pkt);
    }
	fwrite(endcode, 1, sizeof(endcode), file);
	fclose(file);
    avcodec_free_context(&c);
    av_frame_free(&frame);
}
 
/*
Encode one frame from an RGB24 input and save it to the output file.
Must be called after ffmpeg_encoder_start, and ffmpeg_encoder_finish
must be called after the last call to this function.
*/
void ffmpeg_encoder_encode_frame(uint8_t *rgb) {
	int ret, got_output;
    
    /* make sure the frame data is writable */
    ret = av_frame_make_writable(frame);
    if (ret < 0)
        exit(1);
	ffmpeg_encoder_set_frame_yuv_from_rgb(rgb);
	if (frame->pts == 1) {
		frame->key_frame = 1;
		frame->pict_type = AV_PICTURE_TYPE_I;
	}
	else {
		frame->key_frame = 0;
		frame->pict_type = AV_PICTURE_TYPE_P;
	}
	ret = encode(c, &pkt, frame, &got_output);
	if (ret < 0) {
		fprintf(stderr, "Error encoding frame\n");
		exit(1);
	}
	if (got_output) {
		fwrite(pkt.data, 1, pkt.size, file);
		av_packet_unref(&pkt);
	}
}
 
/* Represents the main loop of an application which generates one frame per loop. */
static void encode_example(const char *filename, int codec_id) {
	int pts;
	int width = 320;
	int height = 240;
	uint8_t *rgb = NULL;
	ffmpeg_encoder_start(filename, codec_id, 25, width, height);
	for (pts = 0; pts < 100; pts++) {
		frame->pts = pts;
		rgb = generate_rgb(width, height, pts, rgb);
		ffmpeg_encoder_encode_frame(rgb);
	}
	ffmpeg_encoder_finish();
}
 
int main(void) {
	avcodec_register_all();
	encode_example("tmp.h264", AV_CODEC_ID_H264);
	//encode_example("tmp.mpg", AV_CODEC_ID_MPEG1VIDEO);
	return 0;
}


