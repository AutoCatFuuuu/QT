#ifndef AFILTER_H
#define AFILTER_H

#ifdef __cplusplus
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
}
#endif

class AFilter
{
public:
    AFilter();
    ~AFilter();
    int setSomeArg(const char *filters, AVRational Trational, AVCodecContext *CodeCtx);
    AVFilterContext* getSrcBuffer() { return SrcbufferCtx; }
    AVFilterContext* getSinkBuffer() { return SinkbufferCtx;}
private:
    const AVFilter *Srcbuffer;
    const AVFilter *Sinkbuffer;

    AVFilterInOut *inputs;
    AVFilterInOut *outputs;

    AVFilterContext *SrcbufferCtx;
    AVFilterContext *SinkbufferCtx;

    AVFilterGraph *GrapFilter;
};

#endif // AFILTER_H
