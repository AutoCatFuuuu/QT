/************************************************************
 * 操作流程:
 * 1. open();
 *      打开音视频文件
 *      读取音视频流
 *      读取音视频的解码器
 *      初始化音视频的缓存区
 *      获取音视频的时长,时基
 * 2. readVideo(); readAudio();
 *      解码音视频流
 * 3. getFrame(); getAudio();
 *      读取解码后的音视频流数据
 * 4. seek();
 *      跳转音视频流
 * 5. getDeviation();
 *      计算当前音视频帧的时间差
 ************************************************************/

#ifndef AFFMPEG_H
#define AFFMPEG_H

#ifdef __cplusplus
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libswresample/swresample.h"
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <pthread.h>
}
#endif

#include "afilter.h"

class AFFmpeg
{
public:
    AFFmpeg();
    ~AFFmpeg();
    bool open(const char* filepath,bool isaudio = true);//打开文件
    int readVideo();                                    //读取视频
    int readAudio();                                    //读取音频
    int seek(float pos);                                //跳转
    AVFrame* getFrame(){ return pVideoFrameRGB;}        //读取图像
    int getAudio(char **buf);                           //读取音频
    int getDuration(){ return duration;}                //获取总时长
    double getCDuration(){ return cduration;}           //获取当前时长
    int getFPS() { return fps.num;}                     //获取帧率
    double getDeviation();                              //获取当前视频和音频的时差
    void setAFilterOpen() { isAFilteropen = !isAFilteropen; } //弹幕开关
private:
    //视频相关
    AVFormatContext     *pFormatContext;    //文件内容相关信息

    AVCodecContext      *pVideoCodecContext;//编码信息
    AVCodec             *pVideoCodec;       //解码器
    AVFrame             *pVideoFrame;       //一帧视频 源
    AVFrame             *pVideoFrameRGB;    //一帧视频 RGB格式
    AVRational          fps;                //帧率
    struct SwsContext   *pSwsContext;       //转换格式用的结构体
    unsigned char       *out_buffer;        //数据流初始化用的
    int                 videoindex;         //视频流索引
    AVPacket           *pVCPacket;          //当前视频解码包
    AVRational          Vrational;          //视频时间基准

    //音频相关
    AVFormatContext     *pAFormatContext;   //文件内容相关信息

    AVCodecContext      *pAudioCodecContext;//编码信息
    AVCodec             *pAudioCodec;       //解码器
    AVFrame             *pAudioFrame;       //一帧音频
    struct SwrContext   *pSwrContext;       //转换格式用的结构体
    uint8_t             *pAudioBuffer;      //音频输出数据
    int                 ABufLen;            //pAudioBuffer的长度
    int                 audioindex;         //音频流索引
    AVPacket           *pACPacket;          //当前音频解码包
    AVRational          Arational;          //音频时间基准

    //字幕相关   加弹幕不是很可靠  20条的时候CPU占用太高了 大概是方法不对吧
    AFilter             *pAFilter;          //字幕类
    AVFrame             *pFilterFrame;      //输出帧
    bool                isAFilteropen;      //弹幕开关
    int                 speed;              //弹幕移动速度

    //==================================================
    bool                isopen;             //打开标志
    bool                isstart;            //开始解码标志
    bool                isaudio;            //开启音频

    int                 duration;           //时长
    double              cduration;          //当前时长
};

#endif // AFFMPEG_H
