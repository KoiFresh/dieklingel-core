#ifndef __CAMERA_CAMERA_HPP__
#define __CAMERA_CAMERA_HPP__

#include <linphone/linphonecore.h>
#include <mediastreamer2/msfactory.h>

#include <QJSValue>
#include <QTextStream>
#include <QtConcurrent>
#include <gsl/gsl>
#include <linphone++/linphone.hh>

#include "../setup/Configuration.hpp"
#include "../video/SplitterCamera.hpp"
#include "Capturer.hpp"

class Camera : public Configuration {
    Q_OBJECT
   private:
    std::shared_ptr<linphone::Core> _core;
    Capturer _capturer;
    QString _device;
    QMutex _mutex;
    bool _isSetupCompleted = false;

   public:
    Camera(const Camera &) = delete;
    Camera &operator=(const Camera &) = delete;
    Camera(const Camera &&) = delete;
    Camera &operator=(const Camera &&) = delete;
    Camera(std::shared_ptr<linphone::Core> core);
    ~Camera() override;

    Q_INVOKABLE void device(QString device);
    Q_INVOKABLE void takeB64Snapshot(QJSValue callback);

    void onSetupCompleted() override;
    void print(QTextStream &log) override;
};

#endif  // __CAMERA_CAMERA_HPP__