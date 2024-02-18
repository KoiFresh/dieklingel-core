#include "App.hpp"

App::App(int &argc, char **argv, CoreConfig &settings) : _argc(argc), _argv(argv), _settings(settings)
{
	_initEnv();
	if (this->_settings.getCoreSipEnabled())
	{
		_initCore();
	}
	if (this->_settings.getCoreMqttEnabled())
	{
		_initMqtt();
	}
	_initApplication();

	printCoreInformation();
}

App::~App()
{
}

void App::_initEnv()
{
	QMap env = this->_settings.getCoreEnv();
	for (auto entry = env.cbegin(), end = env.cend(); entry != end; entry++)
	{
		qputenv(entry.key().toLatin1(), entry.value().toLatin1());
	}
}

void App::_initApplication()
{
	if (this->_settings.getCoreQmlEnabled())
	{
		this->_application = std::make_shared<QGuiApplication>(this->_argc, this->_argv);
		this->_engine = std::make_shared<QQmlApplicationEngine>();
		this->_filter = std::make_shared<InactivityDetector>();

		this->_application->installEventFilter(this->_filter.get());
		connect(this->_filter.get(), &InactivityDetector::inactivity, this, &App::inactivity);
	}
	else
	{
		this->_application = std::make_shared<QCoreApplication>(this->_argc, this->_argv);
	}
}

void App::_initCore()
{
	auto factory = Factory::get();
	auto path = QDir::currentPath().toStdString();
	factory->setDataResourcesDir(path);
	factory->setImageResourcesDir(path);
	factory->setTopResourcesDir(path);

	this->_core = factory->createCore("", "", nullptr);
	auto address = factory->createAddress(this->_settings.getCoreSipAddress().toStdString());
	auto info = factory->createAuthInfo(address->getUsername(), "", this->_settings.getCoreSipPassword().toStdString(), "", "", "");
	this->_core->addAuthInfo(info);

	auto proxy = this->_core->createProxyConfig();
	proxy->edit();
	proxy->setIdentityAddress(address);
	proxy->setServerAddr(address->getDomain());
	proxy->enableRegister(true);
	proxy->done();
	this->_core->addProxyConfig(proxy);
	this->_core->setDefaultProxyConfig(proxy);

	this->_core->enableSelfView(false);
	this->_core->enableVideoCapture(true);
	this->_core->enableVideoDisplay(false);

	// disable all beeping indication sounds
	// https://github.com/BelledonneCommunications/linphone-desktop/issues/663#issuecomment-1396680741
	this->_core->getConfig()->setBool("sound", "tone_indications", false);

	this->_core->setCaptureDevice(this->_settings.getCoreSipCaptureDevice().toStdString());
	this->_core->setPlaybackDevice(this->_settings.getCoreSipPlaybackDevice().toStdString());

	auto nat = this->_core->createNatPolicy();
	nat->setStunServer("stun.linphone.org");
	nat->enableStun(true);
	nat->enableIce(true);
	this->_core->setNatPolicy(nat);

	auto transports = factory->createTransports();
	auto t = this->_settings.getCoreSipTransports();
	transports->setUdpPort(t.contains("udp", Qt::CaseInsensitive) ? -1 : 0);
	transports->setDtlsPort(t.contains("dtls", Qt::CaseInsensitive) ? -1 : 0);
	transports->setTcpPort(t.contains("tcp", Qt::CaseInsensitive) ? -1 : 0);
	transports->setTlsPort(t.contains("tls", Qt::CaseInsensitive) ? -1 : 0);
	this->_core->setTransports(transports);

	auto self = std::shared_ptr<CoreListener>(this);
	this->_core->addListener(self);

	auto state = this->_core->start();
	if (state != 0)
	{
		qCritical() << "the linphone core could not be started successfully!";
		exit(state);
	}
}

void App::_initMqtt()
{
	this->_mqtt = std::make_shared<Mqtt>(this->_settings.getCoreMqttAddress());
	auto username = this->_settings.getCoreMqttUsername();
	auto password = this->_settings.getCoreMqttPassword();
	this->_mqtt->connect(username, password);
	for (auto topic : this->_settings.getCoreMqttSubscriptions())
	{
		this->_mqtt->subscribe(topic);
	}
	connect(this->_mqtt.get(), &Mqtt::messageReceived, this, &App::messageReceived);
}

void App::_iterate()
{
	this->_core->iterate();
}

void App::ring(QString number)
{
	qInfo() << "emit ring event";

	this->_ring(number);
	this->publish("dieklingel/core/event/on/ring", number);
}

void App::publish(QString topic, QString message)
{
	if (this->_mqtt == nullptr)
	{
		return;
	}

	this->_mqtt->publish(topic, message);
}

QString App::env(QString key)
{
	return qgetenv(key.toStdString().c_str());
}

void App::_ring(QString number)
{
	if (this->_core == nullptr)
	{
		return;
	}

	if (this->_core->getCurrentCall() != nullptr)
	{
		qInfo() << "there is already an ongoing call, we will not invite" << number;
		return;
	}

	auto params = this->_core->createCallParams(NULL);
	params->enableEarlyMediaSending(true);
	params->setMediaEncryption(MediaEncryption::SRTP);

	this->_core->inviteWithParams(number.toStdString(), params);
}

int App::exec()
{
	QTimer timer = QTimer();
	if (this->_core != nullptr)
	{
		connect(&timer, &QTimer::timeout, this, &App::_iterate);
		timer.start(0);
	}
	if (this->_engine != nullptr)
	{
		const QUrl url = this->_settings.getCoreQmlEntry();
		this->_engine->load(url);
	}

	int code = this->_application->exec();

	if (this->_core != nullptr)
	{
		this->_core->stop();
	}
	return code;
}

void App::printCoreInformation()
{
	QTextStream info(stdout);

	info << "dieklingel core version: " << getVersion() << Qt::endl;
	info << "liblinphone version: " << this->_core->getVersion().c_str() << Qt::endl;
	info << "config file: " << this->_settings.getConfigFilePath() << Qt::endl;

	info << "qml enabled: " << (this->_settings.getCoreQmlEnabled() ? "true" : "false") << Qt::endl;
	if (this->_settings.getCoreQmlEnabled())
	{
		info << "\tentry file: " << this->_settings.getCoreQmlEntry().toString() << Qt::endl;
	}

	info << "mqtt enabled: " << (this->_settings.getCoreMqttEnabled() ? "true" : "false") << Qt::endl;
	if (this->_settings.getCoreMqttEnabled())
	{
		info << "\tmqtt address: " << this->_settings.getCoreMqttAddress() << Qt::endl;
	}

	info << "sip enabled:" << (this->_settings.getCoreSipEnabled() ? "true" : "false") << Qt::endl;
	if (this->_settings.getCoreSipEnabled())
	{
		info << "\taudio codecs:" << Qt::endl;
		for (auto codec : this->_core->getAudioPayloadTypes())
		{
			info << "\t\t- " << codec->getDescription().c_str() << Qt::endl;
		}

		info << "\tvideo codecs:" << Qt::endl;
		for (auto codec : this->_core->getVideoPayloadTypes())
		{
			info << "\t\t- " << codec->getDescription().c_str() << Qt::endl;
		}

		info << "\taudio devices:" << Qt::endl;
		for (auto device : this->_core->getSoundDevicesList())
		{
			info << "\t\t- " << device.c_str();
			if (device == this->_core->getPlaybackDevice())
			{
				info << " (selected [playback])";
			}
			if (device == this->_core->getCaptureDevice())
			{
				info << " (selected [capture])";
			}
			info << Qt::endl;
		}

		info << "\tvideo devices:" << Qt::endl;
		for (auto device : this->_core->getVideoDevicesList())
		{
			info << "\t\t- " << device.c_str();
			if (device == this->_core->getVideoDevice())
			{
				info << " (selected)";
			}
			info << Qt::endl;
		}

		info << "\tdirectories:" << Qt::endl;
		info << "\t\t- config: " << Factory::get()->getConfigDir(nullptr).c_str() << Qt::endl;
		info << "\t\t- data: " << Factory::get()->getDataDir(nullptr).c_str() << Qt::endl;
		info << "\t\t- data resources: " << Factory::get()->getDataResourcesDir().c_str() << Qt::endl;
		info << "\t\t- download: " << Factory::get()->getDownloadDir(nullptr).c_str() << Qt::endl;
		info << "\t\t- image resources: " << Factory::get()->getImageResourcesDir().c_str() << Qt::endl;
		info << "\t\t- mediastreamer plugins: " << Factory::get()->getMspluginsDir().c_str() << Qt::endl;
		info << "\t\t- ring resources: " << Factory::get()->getRingResourcesDir().c_str() << Qt::endl;
		info << "\t\t- sound resources: " << Factory::get()->getSoundResourcesDir().c_str() << Qt::endl;
		info << "\t\t- top resources: " << Factory::get()->getTopResourcesDir().c_str() << Qt::endl;
		info << "\tports:" << Qt::endl;
		info << "\t\tudp: " << this->_core->getTransportsUsed()->getUdpPort() << Qt::endl;
		info << "\t\tdtls: " << this->_core->getTransportsUsed()->getDtlsPort() << Qt::endl;
		info << "\t\ttcp: " << this->_core->getTransportsUsed()->getTcpPort() << Qt::endl;
		info << "\t\ttls: " << this->_core->getTransportsUsed()->getTlsPort() << Qt::endl;
	}
}

QString App::getVersion()
{
#ifndef DIEKLINGEL_CORE_VERSION
	return "unknown";
#else
	return DIEKLINGEL_CORE_VERSION;
#endif
}

void App::onGlobalStateChanged(const std::shared_ptr<linphone::Core> &lc, linphone::GlobalState gstate, const std::string &message)
{
	qInfo() << "Core state changed:" << message.c_str();
};

void App::onRegistrationStateChanged(const std::shared_ptr<Core> &lc, const std::shared_ptr<ProxyConfig> &cfg, RegistrationState cstate, const std::string &message)
{
	qInfo() << "Registration state changed [" << cfg->getIdentityAddress()->asString().c_str() << "]:" << message.c_str();
}

void App::onCallStateChanged(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, linphone::Call::State cstate, const std::string &message)
{
	qInfo() << "Call state changed [" << call->getRemoteAddressAsString().c_str() << "]:" << message.c_str();
	switch (cstate)
	{
	case Call::State::IncomingReceived:
	{
		auto state = call->accept();
		if (state != 0)
		{
			qWarning() << "Failed to accept an incomming call";
		}
		break;
	}
	case Call::State::OutgoingEarlyMedia:
		// mute speaker while early media, in order to suppress the remote
		// ringing sound played by the pbx (like fritzbox)
		call->setSpeakerMuted(true);
		break;
	case Call::State::PausedByRemote:
	case Call::State::Paused:
		call->setSpeakerMuted(true);
		break;
	case Call::State::StreamsRunning:
		call->setSpeakerMuted(false);
		break;
	default:
		break;
	}
}

void App::onDtmfReceived(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, int dtmf)
{
	qInfo() << "Dtmf received" << dtmf;
}
