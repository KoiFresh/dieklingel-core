#ifndef __CAMERA_CAMERA_HPP__
#define __CAMERA_CAMERA_HPP__

#include <linphone/linphonecore.h>
#include <mediastreamer2/msfactory.h>

#include <QJSValue>
#include <QTextStream>
#include <linphone++/linphone.hh>

#include "../setup/Configuration.hpp"
#include "../video/SplitterCamera.hpp"

class Camera : public Configuration {
    Q_OBJECT
   private:
    std::shared_ptr<linphone::Core> _core;
    QString _device;

   public:
    Camera(std::shared_ptr<linphone::Core>);
    ~Camera();

    Q_INVOKABLE void device(QString device);
    Q_INVOKABLE void takeB64Snapshot(QJSValue callback);

    void onSetupCompleted() override;
    void print(QTextStream& log);
};

#endif  // __CAMERA_CAMERA_HPP__