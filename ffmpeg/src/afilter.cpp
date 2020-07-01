#include "afilter.h"

AFilter::AFilter()
{
    avfilter_register_all();

    Srcbuffer = avfilter_get_by_name("buffer");
    Sinkbuffer = avfilter_get_by_name("buffersink");

    inputs = avfilter_inout_alloc();
    outputs = avfilter_inout_alloc();

    GrapFilter = avfilter_graph_alloc();
}

AFilter::~AFilter()
{
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);
    avfilter_free(SrcbufferCtx);
    avfilter_free(SinkbufferCtx);
    avfilter_graph_free(&GrapFilter);
}

int AFilter::setSomeArg(const char *filters,AVRational Trational,AVCodecContext *CodeCtx)
{
    int ret = 0;

    enum AVPixelFormat pix_fmts[] = { CodeCtx->pix_fmt, AV_PIX_FMT_NONE };
//    AVBufferSinkParams *SinkbufferParams = av_buffersink_params_alloc();
//    SinkbufferParams->pixel_fmts = pix_fmts;

    char args[512] = {0};
    snprintf(args,sizeof(args),
             "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
             CodeCtx->width,CodeCtx->height,CodeCtx->pix_fmt,Trational.num,Trational.den,
             CodeCtx->sample_aspect_ratio.num,CodeCtx->sample_aspect_ratio.den);
    ret = avfilter_graph_create_filter(&SrcbufferCtx,Srcbuffer,"in",args,NULL,GrapFilter);

    if(ret < 0)
        goto END;

    ret = avfilter_graph_create_filter(&SinkbufferCtx,Sinkbuffer,"out",NULL,NULL,GrapFilter);
    if(ret < 0)
        goto END;

    ret = av_opt_set_int_list(SinkbufferCtx,"pix_fmts",pix_fmts,AV_PIX_FMT_NONE,AV_OPT_SEARCH_CHILDREN);
    if(ret < 0)
        goto END;

    outputs->name = av_strdup("in");
    outputs->filter_ctx = SrcbufferCtx;
    outputs->pad_idx = 0;
    outputs->next = NULL;

    inputs->name = av_strdup("out");
    inputs->filter_ctx = SinkbufferCtx;
    inputs->pad_idx = 0;
    inputs->next = NULL;


    ret = avfilter_graph_parse_ptr(GrapFilter,filters,&inputs,&outputs,NULL);
    if(ret < 0)
        goto END;

    ret = avfilter_graph_config(GrapFilter,NULL);

END:
    //av_free(SinkbufferParams);
    return ret;
}


