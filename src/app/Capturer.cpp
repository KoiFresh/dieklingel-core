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
		this->_factory = nullptr;
	}
}

void Capturer::useCore(std::shared_ptr<Core> core)
{
	if (this->_core == nullptr)
	{
		ms_factory_destroy(this->_factory);
		this->_factory = nullptr;
	}

	this->_core = core;
	this->_factory = linphone_core_get_ms_factory(core->cPtr());
}

/**
 * create a pixelconverter which should be linked between the source and the
 * subsequent filter. Caused by the fact that the mediastreamer2 has nearly no
 * documentation and only very rare examples, this derived from mediastreamer2
 * `static void _configure_video_preview_source(VideoPreview *stream, bool_t change_source)`
 * See: https://gitlab.linphone.org/BC/public/mediastreamer2/-/blob/master/src/voip/videostream.c#L2123
 *
 * @param source the msfilter which represents the source of the graph
 * @return an pixelconverter msfilter which should be linked to the source
 */
MSFilter *Capturer::_configure(MSFilter *source)
{
	MSFilter *pixconv = nullptr;

	MSPixFmt format;
	MSVideoSize vsize = {
		.width = 640,
		.height = 480,
	};
	float fps = (float)29.97;

	// use this for the possibility to rotate the camera
	/*if (ms_filter_has_method(source, MS_VIDEO_CAPTURE_SET_DEVICE_ORIENTATION))
		ms_filter_call_method(source, MS_VIDEO_CAPTURE_SET_DEVICE_ORIENTATION, 0);
	if (ms_filter_has_method(source, MS_VIDEO_DISPLAY_SET_DEVICE_ORIENTATION))
	{
		ms_filter_call_method(source, MS_VIDEO_DISPLAY_SET_DEVICE_ORIENTATION, 0);
	}*/

	if (!ms_filter_implements_interface(source, MSFilterVideoEncoderInterface))
	{
		ms_filter_call_method(source, MS_FILTER_SET_VIDEO_SIZE, &vsize);
		if (ms_filter_get_id(source) != MS_STATIC_IMAGE_ID)
		{
			ms_filter_call_method(source, MS_FILTER_SET_FPS, &fps);
		}
		ms_filter_call_method(source, MS_FILTER_GET_VIDEO_SIZE, &vsize);
	}
	else
	{
		MSVideoConfiguration vconf;
		ms_filter_call_method(source, MS_VIDEO_ENCODER_GET_CONFIGURATION, &vconf);
		vconf.vsize = vsize;
		vconf.fps = fps;
		ms_filter_call_method(source, MS_VIDEO_ENCODER_SET_CONFIGURATION, &vconf);
	}
	ms_filter_call_method(source, MS_FILTER_GET_PIX_FMT, &format);
	if (format == MS_MJPEG)
	{
		pixconv = ms_factory_create_filter(this->_factory, MS_MJPEG_DEC_ID);
		if (pixconv == NULL)
		{
			qWarning() << "Could not create mjpeg decoder, check your build options.";
		}
	}
	else if (!ms_filter_implements_interface(source, MSFilterVideoEncoderInterface))
	{
		pixconv = ms_factory_create_filter(this->_factory, MS_PIX_CONV_ID);
		ms_filter_call_method(pixconv, MS_FILTER_SET_PIX_FMT, &format);
		ms_filter_call_method(pixconv, MS_FILTER_SET_VIDEO_SIZE, &vsize);
	}
	return pixconv;
}

void Capturer::snapshot()
{
	if (!this->_mutex.tryLock())
	{
		return;
	}

	MSWebCam *webcam = nullptr;
	if (this->_core != nullptr)
	{
		webcam = ms_web_cam_manager_get_cam(ms_factory_get_web_cam_manager(this->_factory), this->_core->getVideoDevice().c_str());
	}
	if (webcam == nullptr)
	{
		webcam = ms_web_cam_manager_get_default_cam(ms_factory_get_web_cam_manager(this->_factory));
	}
	this->_source = ms_web_cam_create_reader(webcam);
	this->_pixconv = _configure(this->_source);
	this->_sink = ms_factory_create_filter(this->_factory, MS_JPEG_WRITER_ID);
	this->_ticker = ms_ticker_new();

	ms_filter_add_notify_callback(this->_sink, Capturer::_onSnapshotTaken, this, false);

	ms_filter_link(this->_source, 0, this->_pixconv, 0);
	ms_filter_link(this->_pixconv, 0, this->_sink, 0);

	ms_ticker_attach(this->_ticker, this->_source);

	ms_filter_call_method(this->_sink, MS_JPEG_WRITER_TAKE_SNAPSHOT, (void *)Capturer::FILENAME.toStdString().c_str());
}

void Capturer::_finishSnapshot()
{
	ms_ticker_detach(this->_ticker, this->_source);

	ms_filter_unlink(this->_source, 0, this->_pixconv, 0);

	// dont do this here. This leads to crash somethimes when calling core->iterate, and I dont know why
	// ms_filter_clear_notify_callback(this->_sink);

	ms_ticker_destroy(this->_ticker);
	this->_ticker = nullptr;
	ms_filter_destroy(this->_sink);
	this->_sink = nullptr;
	ms_filter_destroy(this->_pixconv);
	this->_pixconv = nullptr;
	ms_filter_destroy(this->_source);
	this->_source = nullptr;

	QFile file(Capturer::FILENAME);
	file.open(QIODevice::ReadOnly);
	const QByteArray contents = file.readAll().toBase64();
	file.close();
	_mutex.unlock();

	emit snapshotTaken("data:image/jpeg;base64," + contents);
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
