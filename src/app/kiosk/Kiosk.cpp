
#include "Kiosk.hpp"

Kiosk::Kiosk(std::shared_ptr<Core::Setup> setup) : _setup(std::move(setup)) {}

Kiosk::~Kiosk() = default;

void Kiosk::onSetupCompleted() {
    if (this->_uri.isEmpty()) {
        return;
    }

    this->_setup->useGui();
    QGuiApplication::instance()->installEventFilter(&_detector);
    connect(&this->_detector, &InactivityDetector::inactivity, this, &Kiosk::inactivity);

    qmlRegisterSingletonInstance("com.dieklingel", 1, 0, "App", this);
    this->_setup->engine()->load(_uri);
}

void Kiosk::platform(QString platform) { qputenv("QT_QPA_PLATFORM", platform.toLatin1()); }

void Kiosk::entry(QString uri) { this->_uri = uri; }

void Kiosk::print(QTextStream& info) {
    info << "📺 Kiosk enabled: " << Qt::endl;
    info << "\tentry file: " << this->_uri << Qt::endl;
}
