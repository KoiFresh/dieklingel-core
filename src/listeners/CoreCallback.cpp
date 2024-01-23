

#include "CoreCallback.hpp"

CoreCallback::CoreCallback()
{
}

CoreCallback::~CoreCallback()
{
}

void CoreCallback::onGlobalStateChanged(const std::shared_ptr<linphone::Core> &lc, linphone::GlobalState gstate, const std::string &message)
{
	qInfo() << "Core state" << message.c_str();
};

void CoreCallback::onRegistrationStateChanged(const std::shared_ptr<Core> &lc, const std::shared_ptr<ProxyConfig> &cfg, RegistrationState cstate, const std::string &message)
{
	qInfo() << "Registration state: " << message.c_str() << cfg->getIdentityAddress()->asString().c_str();
}

void CoreCallback::onCallStateChanged(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, linphone::Call::State cstate, const std::string &message)
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

void CoreCallback::onDtmfReceived(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, int dtmf)
{
	qInfo() << "Dtmf received" << dtmf;
}