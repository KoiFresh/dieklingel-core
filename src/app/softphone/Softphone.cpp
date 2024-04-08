#include "Softphone.hpp"

Softphone::Softphone(Setup *setup, std::shared_ptr<linphone::Core> core) {
    this->_setup = setup;
    this->_core = core;

    this->_timer.setInterval(20);
    connect(&this->_timer, &QTimer::timeout, this, &Softphone::_iterate);

    this->_core->enableSelfView(false);
    this->_core->enableVideoCapture(true);
    this->_core->enableVideoDisplay(false);
    this->_core->getConfig()->setBool("sound", "tone_indications", false);
    auto nat = this->_core->createNatPolicy();
    nat->setStunServer("stun.linphone.org");
    nat->enableStun(true);
    nat->enableIce(true);
    this->_core->setNatPolicy(nat);

    auto self = std::shared_ptr<CoreListener>(this);
    this->_core->addListener(self);

    // require the Camera is neccessary, in case that the core.js does not
    // configure the camera. But because the camera already configures the video
    // device for the linphone core, no further actions are needed.
    this->_setup->require<Camera>("camera");
}

Softphone::~Softphone() {}

void Softphone::_iterate() { this->_core->iterate(); }

void Softphone::auth(QString username, QString password) {
    if (this->_isSetupCompleted) {
        throw "Cannot set auth after the setup is completed";
    }
    this->_username = username;
    this->_password = password;
}

void Softphone::proxy(QString proxy) {
    if (this->_isSetupCompleted) {
        throw "Cannot set proxy after the setup is completed";
    }
    this->_proxy = proxy;
}

void Softphone::transport(QStringList transports) {
    auto factory = linphone::Factory::get();
    auto transport = factory->createTransports();

    int udp = transports.contains("udp", Qt::CaseInsensitive) ? -1 : 0;
    transport->setUdpPort(udp);

    int dtls = transports.contains("dtls", Qt::CaseInsensitive) ? -1 : 0;
    transport->setDtlsPort(dtls);

    int tcp = transports.contains("tcp", Qt::CaseInsensitive) ? -1 : 0;
    transport->setTcpPort(tcp);

    int tls = transports.contains("tls", Qt::CaseInsensitive) ? -1 : 0;
    transport->setTlsPort(tls);

    this->_core->setTransports(transport);
}

void Softphone::call(QString number) {
    if (this->_core->getCurrentCall() != nullptr) {
        qInfo() << "there is already an ongoing call, we will not invite"
                << number;
        return;
    }

    auto params = this->_core->createCallParams(NULL);
    params->enableEarlyMediaSending(true);
    params->setMediaEncryption(linphone::MediaEncryption::SRTP);

    this->_core->inviteWithParams(number.toStdString(), params);
}

void Softphone::onSetupCompleted() {
    this->_isSetupCompleted = true;
    auto state = this->_core->start();
    if (state != 0) {
        qCritical() << "the linphone core could not be started successfully!";
        exit(state);
    }

    auto factory = linphone::Factory::get();
    auto address = factory->createAddress(
        QString("sip:%1@%2").arg(this->_username, this->_proxy).toStdString()
    );
    auto info = factory->createAuthInfo(
        address->getUsername(), "", this->_password.toStdString(), "", "", ""
    );
    this->_core->addAuthInfo(info);

    auto proxy = this->_core->createProxyConfig();
    proxy->edit();
    proxy->setIdentityAddress(address);
    proxy->setServerAddr(address->getDomain());
    proxy->enableRegister(true);
    proxy->done();
    this->_core->addProxyConfig(proxy);
    this->_core->setDefaultProxyConfig(proxy);
    this->_core->refreshRegisters();

    _timer.start();
}

void Softphone::onGlobalStateChanged(
    const std::shared_ptr<linphone::Core> &lc,
    linphone::GlobalState gstate,
    const std::string &message
) {
    qInfo() << "Core state changed:" << message.c_str();
};

void Softphone::onRegistrationStateChanged(
    const std::shared_ptr<linphone::Core> &lc,
    const std::shared_ptr<linphone::ProxyConfig> &cfg,
    linphone::RegistrationState cstate,
    const std::string &message
) {
    qInfo() << "Registration state changed ["
            << cfg->getIdentityAddress()->asString().c_str()
            << "]:" << message.c_str();
}

void Softphone::onCallStateChanged(
    const std::shared_ptr<linphone::Core> &lc,
    const std::shared_ptr<linphone::Call> &call,
    linphone::Call::State cstate,
    const std::string &message
) {
    qInfo() << "Call state changed ["
            << call->getRemoteAddressAsString().c_str()
            << "]:" << message.c_str();
    switch (cstate) {
        case linphone::Call::State::IncomingReceived: {
            auto state = call->accept();
            if (state != 0) {
                qWarning() << "Failed to accept an incomming call";
            }
            break;
        }
        case linphone::Call::State::OutgoingEarlyMedia:
            // mute speaker while early media, in order to suppress the remote
            // ringing sound played by the pbx (like fritzbox)
            call->setSpeakerMuted(true);
            break;
        case linphone::Call::State::PausedByRemote:
        case linphone::Call::State::Paused:
            call->setSpeakerMuted(true);
            break;
        case linphone::Call::State::StreamsRunning:
            call->setSpeakerMuted(false);
            break;
        default:
            break;
    }
}

void Softphone::onDtmfReceived(
    const std::shared_ptr<linphone::Core> &lc,
    const std::shared_ptr<linphone::Call> &call,
    int dtmf
) {
    qInfo() << "Dtmf received" << dtmf;
}

void Softphone::print(QTextStream &info) {
    info << "📞 SIP enabled:" << Qt::endl;
    info << "\tliblinphone version: " << this->_core->getVersion().c_str()
         << Qt::endl;

    info << "\taudio codecs:" << Qt::endl;
    for (auto codec : this->_core->getAudioPayloadTypes()) {
        info << "\t\t- " << codec->getDescription().c_str() << Qt::endl;
    }

    info << "\tvideo codecs:" << Qt::endl;
    for (auto codec : this->_core->getVideoPayloadTypes()) {
        info << "\t\t- " << codec->getDescription().c_str() << Qt::endl;
    }

    info << "\taudio devices:" << Qt::endl;
    for (auto device : this->_core->getSoundDevicesList()) {
        info << "\t\t- " << device.c_str();
        if (device == this->_core->getPlaybackDevice()) {
            info << " (selected [playback])";
        }
        if (device == this->_core->getCaptureDevice()) {
            info << " (selected [capture])";
        }
        info << Qt::endl;
    }

    info << "\tdirectories:" << Qt::endl;
    info << "\t\t- config: "
         << linphone::Factory::get()->getConfigDir(nullptr).c_str() << Qt::endl;
    info << "\t\t- data: "
         << linphone::Factory::get()->getDataDir(nullptr).c_str() << Qt::endl;
    info << "\t\t- data resources: "
         << linphone::Factory::get()->getDataResourcesDir().c_str() << Qt::endl;
    info << "\t\t- download: "
         << linphone::Factory::get()->getDownloadDir(nullptr).c_str()
         << Qt::endl;
    info << "\t\t- image resources: "
         << linphone::Factory::get()->getImageResourcesDir().c_str()
         << Qt::endl;
    info << "\t\t- mediastreamer plugins: "
         << linphone::Factory::get()->getMspluginsDir().c_str() << Qt::endl;
    info << "\t\t- ring resources: "
         << linphone::Factory::get()->getRingResourcesDir().c_str() << Qt::endl;
    info << "\t\t- sound resources: "
         << linphone::Factory::get()->getSoundResourcesDir().c_str()
         << Qt::endl;
    info << "\t\t- top resources: "
         << linphone::Factory::get()->getTopResourcesDir().c_str() << Qt::endl;
    info << "\tports:" << Qt::endl;
    info << "\t\tudp: " << this->_core->getTransportsUsed()->getUdpPort()
         << Qt::endl;
    info << "\t\tdtls: " << this->_core->getTransportsUsed()->getDtlsPort()
         << Qt::endl;
    info << "\t\ttcp: " << this->_core->getTransportsUsed()->getTcpPort()
         << Qt::endl;
    info << "\t\ttls: " << this->_core->getTransportsUsed()->getTlsPort()
         << Qt::endl;
}
