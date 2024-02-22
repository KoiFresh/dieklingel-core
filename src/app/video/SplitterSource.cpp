#include "SplitterSource.hpp"

static void splitter_init(MSFilter *filter)
{
	qDebug() << "splitter_init";

	SplitterSource::State *s = new SplitterSource::State();
	s->ticker = ms_ticker_new();
	s->source = nullptr;
	s->sinks = new QSet<MSFilter *>();
	filter->data = s;
}

static void splitter_preprocess(MSFilter *filter)
{
	qDebug() << "splitter_preprocess" << filter->ticker;
}

static void splitter_process(MSFilter *filter)
{
	qDebug() << "splitter source process";
	SplitterSource::State *s = (SplitterSource::State *)filter->data;

	mblk_t *im;
	while ((im = ms_queue_get(filter->inputs[0])) != NULL)
	{
		mblk_t *outm;
		int c = 0;
		qDebug() << "number of sinks" << s->sinks->size();
		for (auto i = s->sinks->begin(), end = s->sinks->end(); i != end; i++)
		{
			qDebug() << "splitter source buffer copied";
			if (c == 0)
			{
				outm = im;
			}
			else
			{
				outm = dupmsg(im);
			}
			qDebug() << (*i)->outputs[0];
			ms_queue_put((*i)->outputs[0], outm);
			c++;
		}
	}
	qDebug() << "splitter_process_finished";
}

static void splitter_postprocess(MSFilter *filter)
{
	qDebug() << "splitter_postprocess" << filter->ticker;
}

static void splitter_uninit(MSFilter *filter)
{
	qDebug() << "splitter_uninit // TODO:";

	SplitterSource::State *s = (SplitterSource::State *)filter->data;
	// ms_filter_unlink(s->tee, s->pid, filter, 0);
	delete s;
	filter->data = nullptr;
	// SplitterFilterState *s = (SplitterFilterState *)filter->data;
	// free(s);
	// filter->data = nullptr;
}

static int splitter_set_fps(MSFilter *filter, void *arg)
{
	qDebug() << "set fps";

	SplitterSource::State *s = (SplitterSource::State *)filter->data;
	if (s->source == nullptr)
	{
		qDebug() << "no source";
	}
	return ms_filter_call_method(s->sizeconv, MS_FILTER_SET_FPS, arg);
}

static int splitter_get_fps(MSFilter *filter, void *arg)
{
	qDebug() << "get fps";

	SplitterSource::State *s = (SplitterSource::State *)filter->data;
	return ms_filter_call_method(s->source, MS_FILTER_GET_FPS, arg);
}

static int splitter_set_video_size(MSFilter *filter, void *arg)
{
	qDebug() << "source: set video size";

	SplitterSource::State *s = (SplitterSource::State *)filter->data;
	qDebug() << "s" << s->source;

	return ms_filter_call_method(s->sizeconv, MS_FILTER_SET_VIDEO_SIZE, arg);
}

static int splitter_get_video_size(MSFilter *filter, void *arg)
{
	qDebug() << "get video size";

	SplitterSource::State *s = (SplitterSource::State *)filter->data;
	return ms_filter_call_method(s->source, MS_FILTER_GET_VIDEO_SIZE, arg);
}

static int splitter_get_pix_fmt(MSFilter *filter, void *arg)
{
	qDebug() << "get pix fmt";

	SplitterSource::State *s = (SplitterSource::State *)filter->data;
	return ms_filter_call_method(s->source, MS_FILTER_GET_PIX_FMT, arg);
}

MSFilterMethod SplitterSource::methods[] = {
	{MS_FILTER_SET_FPS, splitter_set_fps},
	{MS_FILTER_SET_VIDEO_SIZE, splitter_set_video_size},
	{MS_FILTER_GET_VIDEO_SIZE, splitter_get_video_size},
	{MS_FILTER_GET_PIX_FMT, splitter_get_pix_fmt},
	{MS_FILTER_GET_FPS, splitter_get_fps},
	{0, NULL}};

MSFilterDesc SplitterSource::description = {
	.id = MS_FILTER_PLUGIN_ID,
	.name = "SplitterSource",
	.category = MS_FILTER_OTHER,
	.ninputs = 1,
	.noutputs = 0,
	.init = splitter_init,
	.preprocess = splitter_preprocess,
	.process = splitter_process,
	.postprocess = splitter_postprocess,
	.uninit = splitter_uninit,
	.methods = SplitterSource::methods};

// static
void SplitterSource::attach(MSFilter *source, MSFilter *sink)
{
	qDebug() << "attach sink to source";
	SplitterSource::State *sourceState = (SplitterSource::State *)source->data;
	sourceState->sinks->insert(sink);
	if (source->ticker == nullptr)
	{
		ms_ticker_attach(sourceState->ticker, source);
	}
}

// static
void SplitterSource::detach(MSFilter *source, MSFilter *sink)
{
	qDebug() << "detach sink from source";

	SplitterSource::State *s = (SplitterSource::State *)source->data;
	s->sinks->remove(sink);
	if (s->sinks->size() == 0)
	{
		ms_ticker_detach(s->ticker, source);
		qDebug() << "detached!";
	}
	else
	{
		qDebug() << "splitter source has children,ticker not detached";
	}
}
