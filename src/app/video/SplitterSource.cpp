
#include "SplitterSource.hpp"

// static
void SplitterSource::_init(MSFilter *filter) {
    auto sourceState = gsl::owner<SplitterSource::State *>(new SplitterSource::State());
    sourceState->sizeconv = ms_factory_create_filter(filter->factory, MS_SIZE_CONV_ID);
    sourceState->splitterSinks = gsl::owner<QSet<MSFilter *> *>(new QSet<MSFilter *>());
    sourceState->cameraReader = nullptr;

    sourceState->parent = filter;
    filter->data = sourceState;
}

// static
void SplitterSource::_process(MSFilter *filter) {
    auto splitterSourceState = static_cast<SplitterSource::State *>(filter->data);
    splitterSourceState->mutex.lock();

    mblk_t *im = {};
    // NOLINTNEXTLINE
    while ((im = ms_queue_get(filter->inputs[0])) != nullptr) {
        mblk_t *outm = {};
        int count = 0;

        // NOLINTNEXTLINE
        for (auto sink = splitterSourceState->splitterSinks->begin(), end = splitterSourceState->splitterSinks->end();
             sink != end;
             sink++) {
            if (count == 0) {
                outm = im;
            } else {
                outm = dupmsg(im);
            }

            // NOLINTNEXTLINE
            ms_queue_put((*sink)->outputs[0], outm);
            count++;
        }
    }

    splitterSourceState->mutex.unlock();
}

// static
void SplitterSource::_uninit(MSFilter *filter) {
    auto splitterSourceState = static_cast<gsl::owner<SplitterSource::State *>>(filter->data);

    delete splitterSourceState;
    filter->data = nullptr;
}

// static
int SplitterSource::_setFps(MSFilter *filter, void *arg) {
    auto splitterSourceState = static_cast<SplitterSource::State *>(filter->data);
    return ms_filter_call_method(splitterSourceState->sizeconv, MS_FILTER_SET_FPS, arg);
}

// static
int SplitterSource::_getFps(MSFilter *filter, void *arg) {
    auto splitterSourceState = static_cast<SplitterSource::State *>(filter->data);
    return ms_filter_call_method(splitterSourceState->cameraReader, MS_FILTER_GET_FPS, arg);
}

// static
int SplitterSource::_setVideoSize(MSFilter *filter, void *arg) {
    auto splitterSourceState = static_cast<SplitterSource::State *>(filter->data);
    return ms_filter_call_method(splitterSourceState->sizeconv, MS_FILTER_SET_VIDEO_SIZE, arg);
}

// static
int SplitterSource::_getVideoSize(MSFilter *filter, void *arg) {
    auto splitterSourceState = static_cast<SplitterSource::State *>(filter->data);
    return ms_filter_call_method(splitterSourceState->cameraReader, MS_FILTER_GET_VIDEO_SIZE, arg);
}

// static
int SplitterSource::_getPixFmt(MSFilter *filter, void *arg) {
    auto splitterSourceState = static_cast<SplitterSource::State *>(filter->data);
    return ms_filter_call_method(splitterSourceState->cameraReader, MS_FILTER_GET_PIX_FMT, arg);
}

SplitterSource::State::~State() {
    this->mutex.lock();

    ms_filter_unlink(this->cameraReader, 0, this->sizeconv, 0);
    ms_filter_unlink(this->sizeconv, 0, this->parent, 0);

    ms_filter_destroy(this->cameraReader);
    this->cameraReader = nullptr;
    ms_filter_destroy(this->sizeconv);
    this->sizeconv = nullptr;

    delete this->splitterSinks;

    this->mutex.unlock();
}

void SplitterSource::State::attach(MSFilter *sink) {
    this->mutex.lock();

    this->splitterSinks->insert(sink);

    if (this->_ticker == nullptr) {
        this->_ticker = ms_ticker_new();
        ms_ticker_attach(this->_ticker, this->parent);
    }

    this->mutex.unlock();
}

void SplitterSource::State::detach(MSFilter *sink) {
    this->mutex.lock();

    this->splitterSinks->remove(sink);

    if (this->splitterSinks->size() == 0) {
        ms_ticker_detach(this->_ticker, this->parent);

        ms_filter_unlink(this->cameraReader, 0, this->sizeconv, 0);
        ms_filter_unlink(this->sizeconv, 0, this->parent, 0);

        ms_filter_destroy(this->cameraReader);
        ms_filter_destroy(this->sizeconv);

        ms_filter_clean_pending_events(this->parent);
        ms_filter_clear_notify_callback(this->parent);

        ms_ticker_destroy(this->_ticker);
        this->_ticker = nullptr;

        // init for next start
        this->cameraReader = ms_web_cam_create_reader(this->sourceCamera);
        this->sizeconv = ms_factory_create_filter(this->parent->factory, MS_SIZE_CONV_ID);

        ms_filter_link(this->cameraReader, 0, this->sizeconv, 0);
        ms_filter_link(this->sizeconv, 0, this->parent, 0);

        MSVideoSize size = {
            .width = DEFAULT_VIDEO_WIDTH,
            .height = DEFAULT_VIDEO_HEIGHT,
        };
        ms_filter_call_method(this->cameraReader, MS_FILTER_SET_VIDEO_SIZE, &size);
        ms_filter_call_method(this->sizeconv, MS_FILTER_SET_VIDEO_SIZE, &size);

        float fps = DEFAULT_VIDEO_FPS;
        if (ms_filter_get_id(this->cameraReader) != MS_STATIC_IMAGE_ID) {
            ms_filter_call_method(this->cameraReader, MS_FILTER_SET_FPS, &fps);
        }
        ms_filter_call_method(this->sizeconv, MS_FILTER_SET_FPS, &fps);
    }

    this->mutex.unlock();
}
