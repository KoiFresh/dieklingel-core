#ifndef __CAPTURER_HPP__
#define __CAPTURER_HPP__

#include <linphone/linphonecore.h>
#include <mediastreamer2/allfilters.h>
#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msjpegwriter.h>
#include <mediastreamer2/msticker.h>
#include <mediastreamer2/msvideo.h>
#include <mediastreamer2/mswebcam.h>

#include <QString>
#include <QtConcurrent>
#include <linphone++/linphone.hh>
#include <memory>

#include "../Constants.hpp"

class Capturer : public QObject {
    Q_OBJECT
   private:
    static const QString FILENAME;

    std::shared_ptr<linphone::Core> _core;
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
    Capturer(std::shared_ptr<linphone::Core> core);
    ~Capturer();

    void snapshot();

   signals:
    void snapshotTaken(QByteArray snapshot);
};

#endif  // __CAPTURER_HPP__
