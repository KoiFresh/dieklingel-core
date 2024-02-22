#include "SplitterSource.hpp"

// static
void SplitterSource::_init(MSFilter *filter)
{
	SplitterSource::State *sourceState = new SplitterSource::State();
	sourceState->sizeconv = ms_factory_create_filter(filter->factory, MS_SIZE_CONV_ID);
	sourceState->splitterSinks = new QSet<MSFilter *>();
	sourceState->cameraReader = nullptr;

	sourceState->parent = filter;
	filter->data = sourceState;
}

// static
void SplitterSource::_process(MSFilter *filter)
{
	SplitterSource::State *splitterSourceState = (SplitterSource::State *)filter->data;

	mblk_t *im;
	while ((im = ms_queue_get(filter->inputs[0])) != NULL)
	{
		mblk_t *outm;
		int count = 0;
		for (auto sink = splitterSourceState->splitterSinks->begin(), end = splitterSourceState->splitterSinks->end(); sink != end; sink++)
		{
			if (count == 0)
			{
				outm = im;
			}
			else
			{
				outm = dupmsg(im);
			}
			ms_queue_put((*sink)->outputs[0], outm);
			count++;
		}
	}
}

// static
void SplitterSource::_uninit(MSFilter *filter)
{
	SplitterSource::State *splitterSourceState = (SplitterSource::State *)filter->data;

	delete splitterSourceState;
	filter->data = nullptr;
}

// static
int SplitterSource::_setFps(MSFilter *filter, void *arg)
{
	SplitterSource::State *splitterSourceState = (SplitterSource::State *)filter->data;
	return ms_filter_call_method(splitterSourceState->sizeconv, MS_FILTER_SET_FPS, arg);
}

// static
int SplitterSource::_getFps(MSFilter *filter, void *arg)
{
	SplitterSource::State *splitterSourceState = (SplitterSource::State *)filter->data;
	return ms_filter_call_method(splitterSourceState->cameraReader, MS_FILTER_GET_FPS, arg);
}

// static
int SplitterSource::_setVideoSize(MSFilter *filter, void *arg)
{
	SplitterSource::State *splitterSourceState = (SplitterSource::State *)filter->data;
	return ms_filter_call_method(splitterSourceState->sizeconv, MS_FILTER_SET_VIDEO_SIZE, arg);
}

// static
int SplitterSource::_getVideoSize(MSFilter *filter, void *arg)
{
	SplitterSource::State *splitterSourceState = (SplitterSource::State *)filter->data;
	return ms_filter_call_method(splitterSourceState->cameraReader, MS_FILTER_GET_VIDEO_SIZE, arg);
}

// static
int SplitterSource::_getPixFmt(MSFilter *filter, void *arg)
{
	SplitterSource::State *splitterSourceState = (SplitterSource::State *)filter->data;
	return ms_filter_call_method(splitterSourceState->cameraReader, MS_FILTER_GET_PIX_FMT, arg);
}

SplitterSource::State::~State()
{
	this->_mutex.lock();

	ms_filter_unlink(this->cameraReader, 0, this->sizeconv, 0);
	ms_filter_unlink(this->sizeconv, 0, this->parent, 0);

	ms_filter_destroy(this->cameraReader);
	this->cameraReader = nullptr;
	ms_filter_destroy(this->sizeconv);
	this->sizeconv = nullptr;

	delete this->splitterSinks;

	this->_mutex.unlock();
}

MSFilterMethod SplitterSource::methods[] = {
	{MS_FILTER_SET_FPS, SplitterSource::_setFps},
	{MS_FILTER_SET_VIDEO_SIZE, SplitterSource::_setVideoSize},
	{MS_FILTER_GET_VIDEO_SIZE, SplitterSource::_getVideoSize},
	{MS_FILTER_GET_PIX_FMT, SplitterSource::_getPixFmt},
	{MS_FILTER_GET_FPS, SplitterSource::_getFps},
	{0, NULL}};

MSFilterDesc SplitterSource::description = {
	.id = MS_FILTER_PLUGIN_ID,
	.name = "SplitterSource",
	.text = "A filter that was created from a SplitterCamera. It could be connected to a SplitterSink and acts like the end of a graph",
	.category = MS_FILTER_OTHER,
	.ninputs = 1,
	.noutputs = 0,
	.init = SplitterSource::_init,
	.process = SplitterSource::_process,
	.uninit = SplitterSource::_uninit,
	.methods = SplitterSource::methods};

void SplitterSource::State::attach(MSFilter *sink)
{
	this->_mutex.lock();

	this->splitterSinks->insert(sink);

	if (this->_ticker == nullptr)
	{
		this->_ticker = ms_ticker_new();
		ms_ticker_attach(this->_ticker, this->parent);
	}

	this->_mutex.unlock();
}

void SplitterSource::State::detach(MSFilter *sink)
{
	this->_mutex.lock();

	this->splitterSinks->remove(sink);

	if (this->splitterSinks->size() == 0)
	{
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
			.width = 640,
			.height = 480,
		};
		ms_filter_call_method(this->cameraReader, MS_FILTER_SET_VIDEO_SIZE, &size);
		ms_filter_call_method(this->sizeconv, MS_FILTER_SET_VIDEO_SIZE, &size);

		float fps = 30.0;
		ms_filter_call_method(this->cameraReader, MS_FILTER_SET_FPS, &fps);
		ms_filter_call_method(this->sizeconv, MS_FILTER_SET_FPS, &fps);
	}

	this->_mutex.unlock();
}
