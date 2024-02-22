#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <QLoggingCategory>

#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msfactory.h>
#include <mediastreamer2/mswebcam.h>
#include <mediastreamer2/mstee.h>
#include <mediastreamer2/msticker.h>
#include <mediastreamer2/allfilters.h>
#include <mediastreamer2/msvideo.h>

#include "SplitterSource.hpp"
#include "SplitterSink.hpp"

typedef struct
{
	MSWebCam *source = nullptr;
	MSFilter *reader = nullptr;
	MSFilter *pixconv = nullptr;
	MSFilter *sizeconv = nullptr;
	MSFilter *splitter = nullptr;
} SplitterCameraState;

/*typedef struct
{
	MSWebCam *source;
	QList<MSFilter *> *readers;
} Splitter;*/

class Camera
{
private:
	static MSWebCamDesc _description;

	static void _webCamDetectFunc(MSWebCamManager *manager);
	static void _webCamInitFunc(MSWebCam *camera);
	static MSFilter *_webCamCreateReaderFunc(MSWebCam *camera);
	static void _webCamUninitFunc(MSWebCam *camera);
	static bool_t _webCamEncodeToMimeType(MSWebCam *camera, const char *mimeType);

public:
	static void init(MSFactory *factory);

	Camera();
	~Camera();
};

#endif
