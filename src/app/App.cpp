#include "App.hpp"

App::App()
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

	// TODO: read from config file
	auto address = factory->createAddress("sip:koifresh@sip.linphone.org");

	auto info = factory->createAuthInfo(address->getUsername(), "", "PASSWORD", "", "", "");
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
	this->_core->enableVideoCapture(false);
	this->_core->enableVideoDisplay(false);

	auto nat = this->_core->createNatPolicy();
	nat->enableStun(true);
	nat->setStunServer("stun.l.google.com:19302");
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