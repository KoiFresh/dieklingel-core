#ifndef __FILTER_HPP__
#define __FILTER_HPP__

#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msfilter.h>
#include <mediastreamer2/msticker.h>
#include <mediastreamer2/msvideo.h>
#include <mediastreamer2/mswebcam.h>

#include <QLoggingCategory>
#include <QMutex>
#include <QSet>

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

    static MSFilterMethod methods[];
    static MSFilterDesc description;
};

#endif  // __FILTER_HPP__
