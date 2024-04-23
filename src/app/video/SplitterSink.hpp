#ifndef __SPLITTER_SINK_HPP__
#define __SPLITTER_SINK_HPP__
// NOLINTBEGIN

#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msfilter.h>
#include <mediastreamer2/msvideo.h>

#include <QLoggingCategory>

#include "SplitterCamera.hpp"
#include "SplitterSource.hpp"

class SplitterSink {
   private:
    static void _init(MSFilter *filter);
    static void _preprocess(MSFilter *filter);
    static void _process(MSFilter *filter);
    static void _postprocess(MSFilter *filter);
    static void _uninit(MSFilter *filter);

    static int _setFps(MSFilter *filter, void *arg);
    static int _getFps(MSFilter *filter, void *arg);
    static int _setVideoSize(MSFilter *filter, void *arg);
    static int _getVideoSize(MSFilter *filter, void *arg);
    static int _getPixFmt(MSFilter *filter, void *arg);

   public:
    class State {
       public:
        MSFilter *splitterSource = nullptr;
        MSFilter *parent = nullptr;
    };

    static inline MSFilterMethod methods[] = {
        {MS_FILTER_SET_FPS, SplitterSink::_setFps},
        {MS_FILTER_SET_VIDEO_SIZE, SplitterSink::_setVideoSize},
        {MS_FILTER_GET_VIDEO_SIZE, SplitterSink::_getVideoSize},
        {MS_FILTER_GET_PIX_FMT, SplitterSink::_getPixFmt},
        {MS_FILTER_GET_FPS, SplitterSink::_getFps},
        {0, nullptr}};

    static inline MSFilterDesc description = {
        .id = MS_FILTER_PLUGIN_ID,
        .name = "SplitterSink",
        .text =
            "A filter that was created from a SplitterSource and acts like a "
            "Source to a graph",
        .category = MS_FILTER_OTHER,
        .ninputs = 0,
        .noutputs = 1,
        .init = SplitterSink::_init,
        .preprocess = SplitterSink::_preprocess,
        .process = SplitterSink::_process,
        .postprocess = SplitterSink::_postprocess,
        .uninit = SplitterSink::_uninit,
        .methods = SplitterSink::methods};
    ;
};

// NOLINTEND
#endif
