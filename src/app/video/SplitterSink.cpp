#include "SplitterSink.hpp"

static void splitter_init(MSFilter *filter)
{
	qDebug() << "splitter sink init";

	SplitterSink::State *s = new SplitterSink::State();
	filter->data = s;
}

static void splitter_preprocess(MSFilter *filter)
{
	qDebug() << "splitter sink preprocess";
	SplitterSink::State *sinkState = (SplitterSink::State *)filter->data;

	SplitterSource::attach(sinkState->source, filter);
}

static void splitter_process(MSFilter *filter)
{
	SplitterSink::State *s = (SplitterSink::State *)filter->data;
	qDebug() << "splitter sink process ";
}

static void splitter_postprocess(MSFilter *filter)
{
	qDebug() << "splitter sink postprocess";
	SplitterSink::State *sinkState = (SplitterSink::State *)filter->data;

	SplitterSource::detach(sinkState->source, filter);
}

static void splitter_uninit(MSFilter *filter)
{
	qDebug() << "splitter sink uninit";

	SplitterSink::State *s = (SplitterSink::State *)filter->data;
	delete s;
	filter->data = nullptr;
	// SplitterFilterState *s = (SplitterFilterState *)filter->data;
	// free(s);
	// filter->data = nullptr;
}

static int splitter_set_fps(MSFilter *filter, void *arg)
{
	qDebug() << "sink: set fps";

	SplitterSink::State *s = (SplitterSink::State *)filter->data;

	return ms_filter_call_method(s->source, MS_FILTER_SET_FPS, arg);
}

static int splitter_get_fps(MSFilter *filter, void *arg)
{
	qDebug() << "sink: get fps";

	SplitterSink::State *s = (SplitterSink::State *)filter->data;
	return ms_filter_call_method(s->source, MS_FILTER_GET_FPS, arg);
}

static int splitter_set_video_size(MSFilter *filter, void *arg)
{
	qDebug() << "sink: set video size";

	SplitterSink::State *s = (SplitterSink::State *)filter->data;
	return ms_filter_call_method(s->source, MS_FILTER_SET_VIDEO_SIZE, arg);
}

static int splitter_get_video_size(MSFilter *filter, void *arg)
{
	qDebug() << "sink: get video size";

	SplitterSink::State *s = (SplitterSink::State *)filter->data;
	return ms_filter_call_method(s->source, MS_FILTER_GET_VIDEO_SIZE, arg);
}

static int splitter_get_pix_fmt(MSFilter *filter, void *arg)
{
	qDebug() << "sink: get pix fmt";

	SplitterSink::State *s = (SplitterSink::State *)filter->data;
	return ms_filter_call_method(s->source, MS_FILTER_GET_PIX_FMT, arg);
}

MSFilterMethod SplitterSink::methods[] = {
	{MS_FILTER_SET_FPS, splitter_set_fps},
	{MS_FILTER_SET_VIDEO_SIZE, splitter_set_video_size},
	{MS_FILTER_GET_VIDEO_SIZE, splitter_get_video_size},
	{MS_FILTER_GET_PIX_FMT, splitter_get_pix_fmt},
	{MS_FILTER_GET_FPS, splitter_get_fps},
	{0, NULL}};

MSFilterDesc SplitterSink::description = {
	.id = MS_FILTER_PLUGIN_ID,
	.name = "SplitterSink",
	.category = MS_FILTER_OTHER,
	.ninputs = 0,
	.noutputs = 1,
	.init = splitter_init,
	.preprocess = splitter_preprocess,
	.process = splitter_process,
	.postprocess = splitter_postprocess,
	.uninit = splitter_uninit,
	.methods = SplitterSink::methods};
