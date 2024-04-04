#include "SplitterSink.hpp"

// static
void SplitterSink::_init(MSFilter *filter) {
  SplitterSink::State *sinkState = new SplitterSink::State();

  sinkState->parent = filter;
  filter->data = sinkState;
}

// static
void SplitterSink::_preprocess(MSFilter *filter) {
  SplitterSink::State *sinkState = (SplitterSink::State *)filter->data;
  SplitterSource::State *sourceState =
      (SplitterSource::State *)sinkState->splitterSource->data;

  sourceState->attach(filter);
}
// static
void SplitterSink::_process(MSFilter *filter) {
  // Nothing to do here
}

// static
void SplitterSink::_postprocess(MSFilter *filter) {
  SplitterSink::State *sinkState = (SplitterSink::State *)filter->data;
  SplitterSource::State *sourceState =
      (SplitterSource::State *)sinkState->splitterSource->data;

  sourceState->detach(filter);
}

// static
void SplitterSink::_uninit(MSFilter *filter) {
  SplitterSink::State *s = (SplitterSink::State *)filter->data;
  delete s;
  filter->data = nullptr;
}

// static
int SplitterSink::_setFps(MSFilter *filter, void *arg) {
  SplitterSink::State *sinkState = (SplitterSink::State *)filter->data;
  return ms_filter_call_method(sinkState->splitterSource, MS_FILTER_SET_FPS,
                               arg);
}

// static
int SplitterSink::_getFps(MSFilter *filter, void *arg) {
  SplitterSink::State *sinkState = (SplitterSink::State *)filter->data;
  return ms_filter_call_method(sinkState->splitterSource, MS_FILTER_GET_FPS,
                               arg);
}

// static
int SplitterSink::_setVideoSize(MSFilter *filter, void *arg) {
  SplitterSink::State *sinkState = (SplitterSink::State *)filter->data;
  return ms_filter_call_method(sinkState->splitterSource,
                               MS_FILTER_SET_VIDEO_SIZE, arg);
}

// static
int SplitterSink::_getVideoSize(MSFilter *filter, void *arg) {
  SplitterSink::State *sinkState = (SplitterSink::State *)filter->data;
  return ms_filter_call_method(sinkState->splitterSource,
                               MS_FILTER_GET_VIDEO_SIZE, arg);
}

// static
int SplitterSink::_getPixFmt(MSFilter *filter, void *arg) {
  SplitterSink::State *sinkState = (SplitterSink::State *)filter->data;
  return ms_filter_call_method(sinkState->splitterSource, MS_FILTER_GET_PIX_FMT,
                               arg);
}

MSFilterMethod SplitterSink::methods[] = {
    {MS_FILTER_SET_FPS, SplitterSink::_setFps},
    {MS_FILTER_SET_VIDEO_SIZE, SplitterSink::_setVideoSize},
    {MS_FILTER_GET_VIDEO_SIZE, SplitterSink::_getVideoSize},
    {MS_FILTER_GET_PIX_FMT, SplitterSink::_getPixFmt},
    {MS_FILTER_GET_FPS, SplitterSink::_getFps},
    {0, NULL}};

MSFilterDesc SplitterSink::description = {
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
