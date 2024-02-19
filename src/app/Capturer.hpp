#ifndef __CAPTURER_HPP__
#define __CAPTURER_HPP__

#include <linphone++/linphone.hh>
#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msjpegwriter.h>
#include <mediastreamer2/msticker.h>
#include <mediastreamer2/msvideo.h>
#include <mediastreamer2/mswebcam.h>
#include <memory>
#include <QString>
#include <QFuture>
#include <QtConcurrent>

using namespace linphone;

#define MAX_PATH_TEMP 255

typedef struct
{
	char filePath[MAX_PATH_TEMP];
} MSJpegWriteEventData;

class Capturer
{
private:
	QMutex _mutex;
	MSFactory *_factory = nullptr;

	MSFilter *_source = nullptr;
	MSFilter *_pixconv = nullptr;
	MSFilter *_sink = nullptr;
	MSTicker *_ticker = nullptr;

	QFutureInterface<QByteArray> _future;

	static void _onSnapshotTaken(void *userdata, MSFilter *f, unsigned int id, void *arg);
	void _finishSnapshot(QString path);

public:
	Capturer();
	~Capturer();

	QFuture<QByteArray> snapshot();
};

#endif // __CAPTURER_HPP__
