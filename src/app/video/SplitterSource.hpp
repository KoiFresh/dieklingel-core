#ifndef __VIDEO_SPLITTERSOURCE_HPP__
#define __VIDEO_SPLITTERSOURCE_HPP__

#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msfilter.h>
#include <mediastreamer2/msticker.h>
#include <mediastreamer2/msvideo.h>
#include <mediastreamer2/mswebcam.h>

#include <QLoggingCategory>
#include <QMutex>
#include <QSet>
#include <gsl/gsl>

#include "../Constants.hpp"
#include "SplitterCamera.hpp"

class SplitterSource {
   private:
    static void _init(MSFilter *filter);
    static void _process(MSFilter *filter);
    static void _uninit(MSFilter *filter);

    static int _setFps(MSFilter *filter, void *arg);
    static int _getFps(MSFilter *filter, void *arg);
    static int _setVideoSize(MSFilter *filter, void *arg);
    static int _getVideoSize(MSFilter *filter, void *arg);
    static int _getPixFmt(MSFilter *filter, void *arg);

   public:
    class State {
       private:
        MSTicker *_ticker = nullptr;

       public:
        ~State();

        QMutex mutex;
        MSWebCam *sourceCamera = nullptr;
        MSFilter *cameraReader = nullptr;
        MSFilter *sizeconv = nullptr;
        QSet<MSFilter *> *splitterSinks = nullptr;
        MSFilter *parent = nullptr;

        void attach(MSFilter *sink);
        void detach(MSFilter *sink);
    };

    static inline MSFilterMethod methods[] = {
        {MS_FILTER_SET_FPS, SplitterSource::_setFps},
        {MS_FILTER_SET_VIDEO_SIZE, SplitterSource::_setVideoSize},
        {MS_FILTER_GET_VIDEO_SIZE, SplitterSource::_getVideoSize},
        {MS_FILTER_GET_PIX_FMT, SplitterSource::_getPixFmt},
        {MS_FILTER_GET_FPS, SplitterSource::_getFps},
        {0, nullptr}};

    static inline MSFilterDesc description = {
        .id = MS_FILTER_PLUGIN_ID,
        .name = "SplitterSource",
        .text =
            "A filter that was created from a SplitterCamera. It could be "
            "connected to a SplitterSink and acts like the end of a graph",
        .category = MS_FILTER_OTHER,
        .ninputs = 1,
        .noutputs = 0,
        .init = SplitterSource::_init,
        .process = SplitterSource::_process,
        .uninit = SplitterSource::_uninit,
        .methods = SplitterSource::methods};
};

#endif  // __VIDEO_SPLITTERSOURCE_HPP__
