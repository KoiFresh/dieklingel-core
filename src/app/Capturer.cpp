#include "Capturer.hpp"

// static
const QString Capturer::FILENAME = QDir::tempPath() + "/dieklingel-snapshot-tmp.jpg";

Capturer::Capturer()
{
	this->_factory = ms_factory_new_with_voip();
}

Capturer::~Capturer()
{
	if (this->_core == nullptr)
	{
		ms_factory_destroy(this->_factory);
	}
}

void Capturer::useCore(std::shared_ptr<Core> core)
{
	if (this->_core == nullptr)
	{
		ms_factory_destroy(this->_factory);
	}
	this->_factory = linphone_core_get_ms_factory(core->cPtr());
}

QFuture<QByteArray> Capturer::snapshot()
{
	if (this->_future.isRunning())
	{
		return _future.future();
	}
	auto webcam = ms_web_cam_manager_get_default_cam(ms_factory_get_web_cam_manager(this->_factory));

	this->_source = ms_web_cam_create_reader(webcam);
	this->_pixconv = ms_factory_create_filter(this->_factory, MS_PIX_CONV_ID);
	this->_sink = ms_factory_create_filter(this->_factory, MS_JPEG_WRITER_ID);
	this->_ticker = ms_ticker_new();

	ms_filter_add_notify_callback(this->_sink, Capturer::_onSnapshotTaken, this, false);

	MSVideoSize vsize;
	ms_filter_call_method(this->_source, MS_FILTER_GET_VIDEO_SIZE, &vsize);
	ms_filter_call_method(this->_pixconv, MS_FILTER_SET_VIDEO_SIZE, &vsize);

	MSPixFmt format;
	ms_filter_call_method(this->_source, MS_FILTER_GET_PIX_FMT, &format);
	ms_filter_call_method(this->_pixconv, MS_FILTER_SET_PIX_FMT, &format);

	float fps = 30;
	ms_filter_call_method(this->_source, MS_FILTER_GET_FPS, &fps);
	ms_filter_call_method(this->_source, MS_FILTER_SET_FPS, &fps);

	ms_filter_link(this->_source, 0, this->_pixconv, 0);
	ms_filter_link(this->_pixconv, 0, this->_sink, 0);

	ms_ticker_attach(this->_ticker, this->_source);

	ms_filter_call_method(this->_sink, MS_JPEG_WRITER_TAKE_SNAPSHOT, (void *)Capturer::FILENAME.toStdString().c_str());

	this->_future = QFutureInterface<QByteArray>();
	this->_future.reportStarted();
	return this->_future.future();
}

void Capturer::_finishSnapshot()
{
	ms_ticker_detach(this->_ticker, this->_source);

	ms_filter_unlink(this->_pixconv, 0, this->_sink, 0);
	ms_filter_unlink(this->_source, 0, this->_pixconv, 0);

	ms_filter_clear_notify_callback(this->_sink);

	ms_ticker_destroy(this->_ticker);
	ms_filter_destroy(this->_sink);
	ms_filter_destroy(this->_pixconv);
	ms_filter_destroy(this->_source);

	QFile file(Capturer::FILENAME);
	file.open(QIODevice::ReadOnly);
	const QByteArray *contents = new QByteArray(file.readAll());
	file.close();
	this->_future.reportFinished(contents);
}

// static
void Capturer::_onSnapshotTaken(void *userdata, MSFilter *f, unsigned int id, void *arg)
{
	// void *arg is of type MSJpegWriteEventData
	Capturer *self = reinterpret_cast<Capturer *>(userdata);
	switch (id)
	{
	case MS_JPEG_WRITER_SNAPSHOT_TAKEN:
	{
		QtConcurrent::run(
			[self]()
			{
				self->_finishSnapshot();
			});
	}
	break;
	default:
		break;
	}
}
