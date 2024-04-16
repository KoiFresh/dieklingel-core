#include "InactivityDetector.hpp"

InactivityDetector::InactivityDetector() {
    this->_timer.setSingleShot(true);
    this->_timer.setInterval(30'000);
    connect(
        &this->_timer,
        &QTimer::timeout,
        this,
        &InactivityDetector::inactivity
    );
    this->_timer.start();
}

InactivityDetector::~InactivityDetector() {}

bool InactivityDetector::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress ||
        event->type() == QEvent::KeyRelease ||
        event->type() == QEvent::TouchBegin ||
        event->type() == QEvent::TouchCancel ||
        event->type() == QEvent::TouchEnd ||
        event->type() == QEvent::TouchUpdate ||
        event->type() == QEvent::MouseButtonDblClick ||
        event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease ||
        event->type() == QEvent::MouseMove) {
        this->_timer.start();
    }

    return QObject::eventFilter(obj, event);
}