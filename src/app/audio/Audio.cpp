#include "Audio.hpp"

Audio::Audio(std::shared_ptr<linphone::Core> core) { this->_core = core; }

Audio::~Audio() {}

void Audio::capture(QString device) {
    this->_core->setCaptureDevice(device.toStdString());
}

void Audio::playback(QString device) {
    this->_core->setPlaybackDevice(device.toStdString());
}

void Audio::onSetupCompleted() {}

void Audio::print(QTextStream &log) {
    log << "🎛️  Audio enabled:" << Qt::endl;
    log << "\taudio devices:" << Qt::endl;
    for (auto device : this->_core->getSoundDevicesList()) {
        log << "\t\t- " << device.c_str();
        if (device == this->_core->getPlaybackDevice()) {
            log << " (selected [playback])";
        }
        if (device == this->_core->getCaptureDevice()) {
            log << " (selected [capture])";
        }
        log << Qt::endl;
    }
}

void Audio::play(QUrl file) {
    this->_core->playLocal(file.toString().toStdString());
}
