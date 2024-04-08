#include "Camera.hpp"

Camera::Camera(std::shared_ptr<linphone::Core> core) { this->_core = core; }

Camera::~Camera() {}

void Camera::device(QString device) {
    if (!this->_device.isEmpty()) {
        throw std::logic_error(
            QString("The camera.device can only be set once. The device is %1.")
                .arg(this->_device)
                .toStdString()
        );
    }
    this->_core->setVideoDevice(device.toStdString().c_str());
}

void Camera::onSetupCompleted() {
    auto factory = linphone_core_get_ms_factory(this->_core->cPtr());
    QString device = this->_core->getVideoDevice().c_str();

    qDebug() << "device" << device;
    device = SplitterCamera::init(factory, device);
    qDebug() << "device" << device;

    this->_core->reloadVideoDevices();
    this->_core->setVideoDevice(device.toStdString().c_str());
}

void Camera::print(QTextStream& log) {
    log << "📷 Camera enabled: " << Qt::endl;
    log << "\tvideo devices:" << Qt::endl;
    for (auto device : this->_core->getVideoDevicesList()) {
        log << "\t\t- " << device.c_str();
        if (device == this->_core->getVideoDevice()) {
            log << " (selected)";
        }
        log << Qt::endl;
    }
}

void Camera::takeB64Snapshot(QJSValue callback) {
    // TODO: take snapshot
}
