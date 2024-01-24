#include "App.hpp"

App::App(QSettings &settings) : _settings(settings)
{
	qInfo() << "-----------------------";
	qInfo() << "|   dieKlingel Core   |";
	qInfo() << "-----------------------";

	auto factory = Factory::get();
	auto path = QDir::currentPath().toStdString();
	factory->setDataResourcesDir(path);
	factory->setImageResourcesDir(path);
	factory->setTopResourcesDir(path);

	qInfo() << "wd:" << path.c_str();
	qInfo() << "config:" << factory->getConfigDir(nullptr).c_str();
	qInfo() << "data:" << factory->getDataDir(nullptr).c_str();
	qInfo() << "ressources:" << factory->getDataResourcesDir().c_str();

	this->_core = factory->createCore("", "", nullptr);

	auto address = factory->createAddress(this->_settings.value("core.sip/address").toString().toStdString());

	auto info = factory->createAuthInfo(address->getUsername(), "", this->_settings.value("core.sip/password").toString().toStdString(), "", "", "");
	this->_core->addAuthInfo(info);

	auto proxy = this->_core->createProxyConfig();
	proxy->edit();
	proxy->setIdentityAddress(address);
	proxy->setServerAddr(address->getDomain());
	proxy->enableRegister(true);
	proxy->done();
	this->_core->addProxyConfig(proxy);
	this->_core->setDefaultProxyConfig(proxy);

	auto types = this->_core->getVideoPayloadTypes();
	qDebug() << "Video types are:";
	for (auto type : types)
	{
		qDebug() << "\t-" << type->getDescription().c_str();
	}
	this->_core->enableSelfView(false);
	// TODO: enable video capture
	this->_core->enableVideoCapture(false);
	this->_core->enableVideoDisplay(false);

	auto nat = this->_core->createNatPolicy();
	nat->setStunServer("stun.linphone.org");
	nat->enableStun(true);
	nat->enableIce(true);
	this->_core->setNatPolicy(nat);

	auto self = std::shared_ptr<CoreListener>(this);
	this->_core->addListener(self);

	auto state = this->_core->start();
	if (state != 0)
	{
		qCritical() << "the linphone core could not be started successfully!";
		exit(state);
	}
}

App::~App()
{
}

void App::iterate()
{
	this->_core->iterate();
}

void App::ring()
{
	qInfo() << "emit ring event";
	// TODO: start outgoing call
	// this->_core->invite("200");
}

std::shared_ptr<Core> App::getCore() const
{
	return this->_core;
}

void App::onGlobalStateChanged(const std::shared_ptr<linphone::Core> &lc, linphone::GlobalState gstate, const std::string &message)
{
	qInfo() << "Core state" << message.c_str();
};

void App::onRegistrationStateChanged(const std::shared_ptr<Core> &lc, const std::shared_ptr<ProxyConfig> &cfg, RegistrationState cstate, const std::string &message)
{
	qInfo() << "Registration state: " << message.c_str() << cfg->getIdentityAddress()->asString().c_str();
}

void App::onCallStateChanged(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, linphone::Call::State cstate, const std::string &message)
{
	qInfo() << "Call state" << message.c_str();
	switch (cstate)
	{
	case Call::State::IncomingReceived:
	{
		auto state = call->accept();
		if (state != 0)
		{
			qWarning() << "Failed to accept an incomming call";
		}
	}
	break;
	default:
		break;
	}
}

void App::onDtmfReceived(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, int dtmf)
{
	qInfo() << "Dtmf received" << dtmf;
}