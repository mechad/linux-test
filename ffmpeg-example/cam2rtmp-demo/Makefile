OUT_APP		 = cam2rtmp
INCLUDE_PATH = /usr/local/include/
INCLUDE = -I$(INCLUDE_PATH)libavutil/ -I$(INCLUDE_PATH)libavdevice/ \
			-I$(INCLUDE_PATH)libavcodec/ -I$(INCLUDE_PATH)libswresample \
			-I$(INCLUDE_PATH)libavfilter/ -I$(INCLUDE_PATH)libavformat \
			-I$(INCLUDE_PATH)libswscale/
 
FFMPEG_LIBS = -lavformat -lavutil -lavdevice -lavcodec -lswresample -lavfilter -lswscale
SDL_LIBS	= 
LIBS		= $(FFMPEG_LIBS)$(SDL_LIBS)
 
COMPILE_OPTS = $(INCLUDE)
C 			 = c
OBJ 		 = o
C_COMPILER   = cc
C_FLAGS 	 = $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS)
 
LINK 		 = cc -o 
LINK_OPTS    = -lz -lm  -lpthread
LINK_OBJ	 = cam2rtmp.o
 
.$(C).$(OBJ):
	$(C_COMPILER) -c -g $(C_FLAGS) $<
 
 
$(OUT_APP): $(LINK_OBJ)
	$(LINK)$@  $(LINK_OBJ)  $(LIBS) $(LINK_OPTS)
 
clean:
	rm -rf *.$(OBJ) $(OUT_APP) core *.core *~  *.jpeg
