#include "Camera.hpp"

// static
MSWebCamDesc Camera::_description = {
	.driver_type = "Splitter",
	.detect = Camera::_webCamDetectFunc,
	.init = Camera::_webCamInitFunc,
	.create_reader = Camera::_webCamCreateReaderFunc,
	.uninit = Camera::_webCamUninitFunc,
	.encode_to_mime_type = Camera::_webCamEncodeToMimeType,
};

// static
void Camera::init(MSFactory *factory)
{
	auto manager = ms_factory_get_web_cam_manager(factory);
	ms_web_cam_manager_register_desc(manager, &Camera::_description);
	Camera::_webCamDetectFunc(manager);

	// ms_factory_register_filter(factory, &description);
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

// static
void Camera::_webCamDetectFunc(MSWebCamManager *manager)
{
	// TODO: read s from set
	QString s = "V4L2: /dev/video0";
	auto source = ms_web_cam_manager_get_cam(manager, s.toStdString().c_str());
	if (source == nullptr)
	{
		return;
	}

	auto camera = ms_web_cam_manager_create_cam(manager, &Camera::_description);
	camera->name = ms_strdup(s.toStdString().c_str());

	SplitterCameraState *splitterCameraState = (SplitterCameraState *)malloc(sizeof(SplitterCameraState));
	splitterCameraState->source = source;
	splitterCameraState->reader = ms_web_cam_create_reader(source);
	splitterCameraState->pixconv = ms_factory_create_filter(manager->factory, MS_PIX_CONV_ID);
	splitterCameraState->sizeconv = ms_factory_create_filter(manager->factory, MS_SIZE_CONV_ID);
	splitterCameraState->splitter = ms_factory_create_filter_from_desc(manager->factory, &SplitterSource::description);
	SplitterSource::State *sourceState = (SplitterSource::State *)splitterCameraState->splitter->data;
	sourceState->source = splitterCameraState->reader;
	sourceState->pixconv = splitterCameraState->pixconv;
	sourceState->sizeconv = splitterCameraState->sizeconv;

	ms_filter_link(splitterCameraState->reader, 0, splitterCameraState->sizeconv, 0);
	// ms_filter_link(splitterCameraState->pixconv, 0, splitterCameraState->sizeconv, 0);
	ms_filter_link(splitterCameraState->sizeconv, 0, splitterCameraState->splitter, 0);

	MSVideoSize size = {
		.width = 640,
		.height = 480,
	};
	ms_filter_call_method(sourceState->source, MS_FILTER_SET_VIDEO_SIZE, &size);
	ms_filter_call_method(sourceState->sizeconv, MS_FILTER_SET_VIDEO_SIZE, &size);

	qDebug() << "size is" << size.width << "x" << size.height;
	float fps = 30.0;
	ms_filter_call_method(sourceState->source, MS_FILTER_SET_FPS, &fps);
	ms_filter_call_method(sourceState->sizeconv, MS_FILTER_SET_FPS, &fps);

	camera->data = splitterCameraState;
	ms_web_cam_manager_add_cam(manager, camera);
}

// static
void Camera::_webCamInitFunc(MSWebCam *camera)
{
	qDebug() << "_webCamInit - nothing to do";
}

// static
MSFilter *Camera::_webCamCreateReaderFunc(MSWebCam *camera)
{
	qDebug() << "reader";
	SplitterCameraState *splitter = reinterpret_cast<SplitterCameraState *>(camera->data);

	auto sink = ms_factory_create_filter_from_desc(camera->wbcmanager->factory, &SplitterSink::description);
	SplitterSink::State *sinkState = (SplitterSink::State *)sink->data;
	sinkState->source = splitter->splitter;

	// SplitterSource::attach(sinkState->source, sink);

	return sink;
}

// static
void Camera::_webCamUninitFunc(MSWebCam *camera)
{
	qDebug() << "uninit";
	SplitterCameraState *splitter = reinterpret_cast<SplitterCameraState *>(camera->data);
	// ms_filter_unlink(splitter->reader, 0, splitter->tee, 0);

	// ms_filter_destroy(splitter->reader);
	//  ms_filter_destroy(splitter->tee);

	free(camera->data);
	camera->data = nullptr;
}

// static
bool_t Camera::_webCamEncodeToMimeType(MSWebCam *camera, const char *mimeType)
{
	qDebug() << "Call to _webCamEncodeToMimeType, which is not implemmented";
	SplitterCameraState *splitter = reinterpret_cast<SplitterCameraState *>(camera->data);
	return false;
	if (splitter->source->desc->encode_to_mime_type != NULL)
	{
		return splitter->source->desc->encode_to_mime_type(splitter->source, mimeType);
	}
}
