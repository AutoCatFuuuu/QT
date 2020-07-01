/*****************************************************************
 * 功能说明:
 *  抓取桌面或系统摄像头图像
 * 注意:
 * 1. setOptions();
 *      这个写着玩的,没调试...
 *****************************************************************/

#ifndef GDIGRAB_H
#define GDIGRAB_H

#ifdef __cplusplus
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavdevice/avdevice.h>
#include <libavutil/opt.h>
}
#endif

#include "aencode.h"

class Gdigrab {
public:
    Gdigrab();
    ~Gdigrab();

    bool open();    //打开
    bool read();    //读取
    void setOptions(int x,int y,int width,int height);
    int getFPS() { return fps.num; }            //获取帧率
    AVFrame* getFrame() { return pFrameRGB; }   //获取图像
    void stop();    //停止编码并关闭文件

private:
    AVDictionary        *options;       //参数
    AVFormatContext     *pFormatCtx;    //文件内容信息 这里指屏幕
    AVCodecContext      *pCodecCtx;     //解码器上下文
    AVCodec             *pCodec;        //解码器
    AVFrame             *pFrame;        //解码出来的图像
    AVFrame             *pFrameRGB;     //转换成RGB的图像
    AVPacket            *packet;        //图像解码前的包
    AVRational          fps;
    unsigned char       *out_buffer;    //图像缓存区
    struct SwsContext   *pSwsContext;   //转换格式
    int                 videoindex;     //视频流索引

    AEncode E;  //编码器

};

#endif
