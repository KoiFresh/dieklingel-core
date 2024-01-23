#include <linphone++/linphone.hh>
#include <QLoggingCategory>
#include <QDir>
#include <QSettings>

using namespace linphone;

class App : public QObject, public CoreListener
{
	Q_OBJECT
private:
	std::shared_ptr<Core> _core;
	QSettings &_settings;

public:
	explicit App(QSettings &settings);
	~App();

	std::shared_ptr<Core> getCore() const;

public slots:
	void iterate();

public:
	Q_INVOKABLE void ring();

	void onGlobalStateChanged(const std::shared_ptr<linphone::Core> &lc, linphone::GlobalState gstate, const std::string &message) override;
	void onRegistrationStateChanged(const std::shared_ptr<Core> &lc, const std::shared_ptr<ProxyConfig> &cfg, RegistrationState cstate, const std::string &message) override;
	void onCallStateChanged(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, linphone::Call::State cstate, const std::string &message) override;
	void onDtmfReceived(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, int dtmf) override;
};
