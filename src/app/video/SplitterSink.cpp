#include "SplitterSink.hpp"

// static
void SplitterSink::_init(MSFilter *filter) {
    auto sinkState = gsl::owner<SplitterSink::State *>(new SplitterSink::State());

    sinkState->parent = filter;
    filter->data = sinkState;
}

// static
void SplitterSink::_preprocess(MSFilter *filter) {
    auto sinkState = static_cast<SplitterSink::State *>(filter->data);
    auto sourceState = static_cast<SplitterSource::State *>(sinkState->splitterSource->data);

    sourceState->attach(filter);
}
// static
void SplitterSink::_process(MSFilter *filter) {
    // Nothing to do here
}

// static
void SplitterSink::_postprocess(MSFilter *filter) {
    auto sinkState = static_cast<SplitterSink::State *>(filter->data);
    auto sourceState = static_cast<SplitterSource::State *>(sinkState->splitterSource->data);

    sourceState->detach(filter);
}

// static
void SplitterSink::_uninit(MSFilter *filter) {
    auto sinkState = static_cast<gsl::owner<SplitterSink::State *>>(filter->data);
    delete sinkState;
    filter->data = nullptr;
}

// static
int SplitterSink::_setFps(MSFilter *filter, void *arg) {
    auto sinkState = static_cast<SplitterSink::State *>(filter->data);
    return ms_filter_call_method(sinkState->splitterSource, MS_FILTER_SET_FPS, arg);
}

// static
int SplitterSink::_getFps(MSFilter *filter, void *arg) {
    auto sinkState = static_cast<SplitterSink::State *>(filter->data);
    return ms_filter_call_method(sinkState->splitterSource, MS_FILTER_GET_FPS, arg);
}

// static
int SplitterSink::_setVideoSize(MSFilter *filter, void *arg) {
    auto sinkState = static_cast<SplitterSink::State *>(filter->data);
    return ms_filter_call_method(sinkState->splitterSource, MS_FILTER_SET_VIDEO_SIZE, arg);
}

// static
int SplitterSink::_getVideoSize(MSFilter *filter, void *arg) {
    auto sinkState = static_cast<SplitterSink::State *>(filter->data);
    return ms_filter_call_method(sinkState->splitterSource, MS_FILTER_GET_VIDEO_SIZE, arg);
}

// static
int SplitterSink::_getPixFmt(MSFilter *filter, void *arg) {
    auto sinkState = static_cast<SplitterSink::State *>(filter->data);
    return ms_filter_call_method(sinkState->splitterSource, MS_FILTER_GET_PIX_FMT, arg);
}
