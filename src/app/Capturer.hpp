#ifndef __CAPTURER_HPP__
#define __CAPTURER_HPP__

#include <linphone++/linphone.hh>
#include <linphone/linphonecore.h>
#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msjpegwriter.h>
#include <mediastreamer2/msticker.h>
#include <mediastreamer2/msvideo.h>
#include <mediastreamer2/mswebcam.h>
#include <mediastreamer2/allfilters.h>
#include <memory>
#include <QString>
#include <QtConcurrent>

using namespace linphone;

class Capturer : public QObject
{
	Q_OBJECT
private:
	static const QString FILENAME;

	std::shared_ptr<Core> _core;
	MSFactory *_factory = nullptr;

	MSFilter *_source = nullptr;
	MSFilter *_pixconv = nullptr;
	MSFilter *_sink = nullptr;
	MSTicker *_ticker = nullptr;

	QMutex _mutex;

	MSFilter *_configure(MSFilter *source);
	static void _onSnapshotTaken(void *userdata, MSFilter *f, unsigned int id, void *arg);
	void _finishSnapshot();

public:
	Capturer();
	~Capturer();

	void useCore(std::shared_ptr<Core> core);
	void snapshot();

signals:
	void snapshotTaken(QByteArray snapshot);
};

#endif // __CAPTURER_HPP__
