#include <linphone++/linphone.hh>
#include <qt6/QtCore/QLoggingCategory>

using namespace linphone;

class CoreCallback : public CoreListener

{
private:
	/* data */
public:
	CoreCallback();
	~CoreCallback();

	void onGlobalStateChanged(const std::shared_ptr<linphone::Core> &lc, linphone::GlobalState gstate, const std::string &message) override;
	void onRegistrationStateChanged(const std::shared_ptr<Core> &lc, const std::shared_ptr<ProxyConfig> &cfg, RegistrationState cstate, const std::string &message) override;
	void onCallStateChanged(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, linphone::Call::State cstate, const std::string &message) override;
	void onDtmfReceived(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, int dtmf) override;
};
