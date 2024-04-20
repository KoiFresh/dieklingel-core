
#include "SplitterCamera.hpp"

// NOLINTNEXTLINE
QString SplitterCamera::_cameraId = "";

// static
QString SplitterCamera::init(MSFactory *factory, QString cameraId) {
    SplitterCamera::_cameraId = cameraId;

    auto manager = ms_factory_get_web_cam_manager(factory);
    ms_web_cam_manager_register_desc(manager, &SplitterCamera::description);
    // for instant detection without reloading all cameras
    // _webCamDetectFunc(manager);
    // Warning: it is not save to call ms_web_cam_mananger_reload in here, after
    // calling ms_web_cam_manager_register_desc the linphone core should reload
    // the video devices. ms_web_cam_manager_reload(manager);/

    return QString(SplitterCamera::description.driver_type) + ": " + cameraId;
}

// static
void SplitterCamera::_webCamDetectFunc(MSWebCamManager *manager) {
    auto sourceCamera = ms_web_cam_manager_get_cam(manager, SplitterCamera::_cameraId.toStdString().c_str());
    if (sourceCamera == nullptr) {
        qWarning() << "The SplitterCamera for" << SplitterCamera::_cameraId
                   << "was not created, because the source camera couldn't be found";
        return;
    }

    auto camera = ms_web_cam_manager_create_cam(manager, &SplitterCamera::description);
    // actual name will be "<drive>: <name>"
    camera->name = ms_strdup(SplitterCamera::_cameraId.toStdString().c_str());

    auto splitterCameraState = gsl::owner<SplitterCamera::State *>(new SplitterCamera::State());
    splitterCameraState->sourceCamera = sourceCamera;
    splitterCameraState->splitterSource =
        ms_factory_create_filter_from_desc(manager->factory, &SplitterSource::description);

    auto sourceState = static_cast<SplitterSource::State *>(splitterCameraState->splitterSource->data);
    sourceState->sourceCamera = sourceCamera;
    sourceState->cameraReader = ms_web_cam_create_reader(sourceCamera);

    ms_filter_link(sourceState->cameraReader, 0, sourceState->sizeconv, 0);
    ms_filter_link(sourceState->sizeconv, 0, splitterCameraState->splitterSource, 0);

    MSVideoSize size = {
        .width = DEFAULT_VIDEO_WIDTH,
        .height = DEFAULT_VIDEO_HEIGHT,
    };
    ms_filter_call_method(sourceState->cameraReader, MS_FILTER_SET_VIDEO_SIZE, &size);
    ms_filter_call_method(sourceState->sizeconv, MS_FILTER_SET_VIDEO_SIZE, &size);

    float fps = DEFAULT_VIDEO_FPS;
    if (ms_filter_get_id(sourceState->cameraReader) != MS_STATIC_IMAGE_ID) {
        ms_filter_call_method(sourceState->cameraReader, MS_FILTER_SET_FPS, &fps);
    }
    ms_filter_call_method(sourceState->sizeconv, MS_FILTER_SET_FPS, &fps);

    camera->data = splitterCameraState;
    ms_web_cam_manager_add_cam(manager, camera);
}

// static
void SplitterCamera::_webCamInitFunc(MSWebCam *camera) {
    // Nothing to do here
}

// static
MSFilter *SplitterCamera::_webCamCreateReaderFunc(MSWebCam *camera) {
    auto splitterCamera = static_cast<SplitterCamera::State *>(camera->data);
    MSFilter *reader = splitterCamera->createSplitterSink();

    return reader;
}

// static
void SplitterCamera::_webCamUninitFunc(MSWebCam *camera) {
    auto splitterCameraState = static_cast<gsl::owner<SplitterCamera::State *>>(camera->data);
    ms_filter_destroy(splitterCameraState->splitterSource);
    splitterCameraState->splitterSource = nullptr;

    delete splitterCameraState;
    camera->data = nullptr;
}

// static
bool_t SplitterCamera::_webCamEncodeToMimeType(MSWebCam *camera, const char *mimeType) {
    /*qDebug() << "Call to _webCamEncodeToMimeType, which is not implemmented";
    SplitterCamera::State *splitter = reinterpret_cast<SplitterCamera::State
    *>(camera->data);*/
    return false;
    /*if (splitter->sourceCamera->desc->encode_to_mime_type != NULL)
    {
            return
    splitter->sourceCamera->desc->encode_to_mime_type(splitter->sourceCamera,
    mimeType);
    }*/
}

MSFilter *SplitterCamera::State::createSplitterSink() {
    MSFactory *factory = this->sourceCamera->wbcmanager->factory;
    MSFilter *sink = ms_factory_create_filter_from_desc(factory, &SplitterSink::description);

    auto sinkState = static_cast<SplitterSink::State *>(sink->data);
    sinkState->splitterSource = this->splitterSource;

    return sink;
}
