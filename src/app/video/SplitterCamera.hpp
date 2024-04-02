#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <mediastreamer2/allfilters.h>
#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msfactory.h>
#include <mediastreamer2/mstee.h>
#include <mediastreamer2/msticker.h>
#include <mediastreamer2/msvideo.h>
#include <mediastreamer2/mswebcam.h>

#include <QLoggingCategory>
#include <QString>

#include "SplitterSink.hpp"
#include "SplitterSource.hpp"

class SplitterCamera {
 private:
  static QString _cameraId;

  static void _webCamDetectFunc(MSWebCamManager *manager);
  static void _webCamInitFunc(MSWebCam *camera);
  static MSFilter *_webCamCreateReaderFunc(MSWebCam *camera);
  static void _webCamUninitFunc(MSWebCam *camera);
  static bool_t _webCamEncodeToMimeType(MSWebCam *camera, const char *mimeType);

 public:
  static MSWebCamDesc description;
  static QString init(MSFactory *factory, QString cameraId);

  class State {
   public:
    MSWebCam *sourceCamera = nullptr;
    MSFilter *splitterSource = nullptr;

    MSFilter *createSplitterSink();
  };
};

#endif
