/************************************************************************************
 * 功能说明:
 *  本demo工程里是实现录取屏幕 视频数据是从gdigrab里抓过来的
 *  想实现 本地文件视频转码 可以先把视频解码出来再传到encodecOneFrame()
 * 注意:
 * 1. 成员函数声明顺序就是编码流程的顺序,请一定要按顺序执行,否则程序会崩溃
 * 2. 本程序目前只支持 单视频流编码
 * 3. 编码格式会根据输出文件名猜测判断
 * 4. 对于编码器的参数设置接口并没有写很多 主要是因为对编码器的参数没啥研究...
 * 5. setPixFormat() 别乱设置 编码器不支持的话,会没法编码的
 *      默认是 AV_PIX_FMT_YUV420P
 * 6. setSize()
 *      默认是 w:1920 h:1080
 * 7. setFPS()
 *      默认 25
 ************************************************************************************/

#ifndef AENCODE_H
#define AENCODE_H

#ifdef __cplusplus
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}
#endif

class AEncode
{
public:
    AEncode();                              //初始化私有成员
    ~AEncode();                             //调用free();
    void setPath(char* path);               //设置文件路径
    void setSize(int width,int height);     //设置视频宽高
    void setFPS(int value) { fps = value; } //设置帧率
    void setPixFormat(AVPixelFormat p) { VPixFormat = p; }
                                            //设置输出图像的颜色空间
    void setSwsCtx(AVPixelFormat src, int src_w, int src_h);
                                            //设置转换格式
    bool initOutFile();                     //为输出文件初始化
    bool initVideo();                       //初始化视频编码器
    bool startEncodec();                    //写文件头
    bool encodecOneFrame(AVFrame *frame, int index);
                                            //编码一帧
    bool endEncodec();                      //写文件尾
    void free();                            //销毁资源

private:
    AVFormatContext     *pFormatCtx;        //多媒体上下文
    AVCodecContext      *pVCodecCtx;        //视频编码器上下文
    AVCodec             *pVCodec;           //视频编码器
    AVOutputFormat      *fmt;               //输出参数
    AVStream            *pVStream;          //视频输出流
    AVFrame             *pVFrame;           //视频编码帧
    AVPacket            *pVPacket;          //视频编码帧
    AVPixelFormat       VPixFormat;         //图像的颜色空间
    struct SwsContext   *pSwsCtx;           //转换格式
    unsigned char       *out_buffer;        //图像缓存区
    char                path[256];          //文件路径
    int                 width;              //宽
    int                 height;             //高
    int                 fps;                //帧率
    int                 videoindex;         //视频流
};

#endif // AENCODE_H
