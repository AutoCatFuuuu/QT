#include "gdigrab.h"
#include "stdio.h"

#define _CAMERA_ 0  //会一直打印警告信息 没发现有什么影响 也不知道怎么处理
#define _ENCODEC_ 1 //编码部分

Gdigrab::Gdigrab()
{
    options = NULL;
    videoindex = -1;
    av_register_all();
    avdevice_register_all();

    pFormatCtx  = avformat_alloc_context();
    pFrame      = av_frame_alloc();
    pFrameRGB   = av_frame_alloc();
    packet      =(AVPacket *)av_malloc(sizeof(AVPacket));
    pCodec      = NULL;
    pCodecCtx   = NULL;
    out_buffer  = NULL;

#if _ENCODEC_
    E.setPath("D:/test.mp4");
    E.setFPS(5);
    //E.setSize(480,270);
    E.initOutFile();
    E.initVideo();
    E.startEncodec();
#endif
}

Gdigrab::~Gdigrab()
{
    sws_freeContext(pSwsContext);
    av_free(pFrameRGB);
    av_free(pFrame);
    av_free_packet(packet);
    av_dict_free(&options);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    av_free(out_buffer);
}

bool Gdigrab::open()
{
    //设置帧率为5
    av_dict_set(&options,"framerate","5",0);

#if _CAMERA_
    //打开摄像头
    AVInputFormat *ifmt = av_find_input_format("vfwcap");
    if(avformat_open_input(&pFormatCtx,"0",ifmt,NULL)!=0){
#else
    AVInputFormat *ifmt = av_find_input_format("gdigrab");
    if(avformat_open_input(&pFormatCtx,"desktop",ifmt,&options)!=0) {
#endif
        printf("Couldn't open input stream.\n");
        return false;
    }
    //查找流信息
    if(avformat_find_stream_info(pFormatCtx,NULL)<0) {
        printf("Couldn't find stream information.\n");
        return false;
    }
    //视频流
    for(int i=0; i<pFormatCtx->nb_streams; i++) {
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
            videoindex=i;
            break;
        }
    }
    if(videoindex==-1) {
        printf("Didn't find a video stream.\n");
        return false;
    }
    //获取帧率
    fps = pFormatCtx->streams[videoindex]->avg_frame_rate;
    //解码器上下文
    pCodecCtx=pFormatCtx->streams[videoindex]->codec;

//    printf("width:%d,h:%d,num:%d,den:%d,bit:%d,gop:%d,min:%d,max:%d\n",
//           pCodecCtx->width,pCodecCtx->height,pCodecCtx->time_base.num,
//           pCodecCtx->time_base.den,pCodecCtx->bit_rate,pCodecCtx->gop_size,
//           pCodecCtx->qmin,pCodecCtx->qmax);
    //查找解码器
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL) {
        printf("Codec not found.\n");
        return false;
    }
    //打开解码器
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0) {
        printf("Could not open codec.\n");
        return false;
    }
    //创建图像缓冲区
    out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32,pCodecCtx->width,pCodecCtx->height,1));
    av_image_fill_arrays(pFrameRGB->data,pFrameRGB->linesize,out_buffer,
                         AV_PIX_FMT_RGB32,pCodecCtx->width,pCodecCtx->height,1);
    //设置转换格式
    pSwsContext = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                 pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32,
                                 SWS_BICUBIC, NULL, NULL, NULL);
#if _ENCODEC_
    E.setSwsCtx(pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);
#endif
    return true;
}

bool Gdigrab::read()
{
    int got_picture = 0;
    static int index = 0;
    if(av_read_frame(pFormatCtx, packet) >= 0) {
        if(packet->stream_index == videoindex) {
            if(avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet) < 0) {
                printf("Decode Error.\n");
                return false;
            }
            if(got_picture){
                sws_scale(pSwsContext,(const unsigned char* const*)pFrame->data,pFrame->linesize,0,
                          pCodecCtx->height,pFrameRGB->data,pFrameRGB->linesize);
                pFrameRGB->width = pCodecCtx->width;
                pFrameRGB->height = pCodecCtx->height;
#if _ENCODEC_
                pFrame->height = pCodecCtx->height;
                pFrame->pts = index ;
                index++;
                E.encodecOneFrame(pFrame,index);
#endif
                av_free_packet(packet);

                return true;
            }
        }
    }
    return false;
}

void Gdigrab::setOptions(int x, int y, int width, int height)
{
    //The distance from the left edge of the screen or desktop
    //av_dict_set(&options,"offset_x","20",0);
    //The distance from the top edge of the screen or desktop
    //av_dict_set(&options,"offset_y","40",0);
    //Video frame size. The default is to capture the full screen
    //av_dict_set(&options,"video_size","640x480",0);
    char offset_x[8] = {0};
    sprintf(offset_x,"%d",x);

    char offset_y[8] = {0};
    sprintf(offset_y,"%d",y);

    char video_size[16] = {0};
    sprintf(video_size,"%dx%d",width,height);

    av_dict_set(&options,"offset_x",offset_x,0);
    av_dict_set(&options,"offset_y",offset_y,0);
    av_dict_set(&options,"video_size",video_size,0);
}

void Gdigrab::stop()
{
#if _ENCODEC_
    E.endEncodec();
    E.free();
#endif
}
