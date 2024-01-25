#ifndef __APP_HPP__
#define __APP_HPP__

#include <linphone++/linphone.hh>
#include <QLoggingCategory>
#include <QDir>
#include <QSettings>

#include "Mqtt.hpp"

using namespace linphone;

class App : public QObject, public CoreListener
{
	Q_OBJECT
private:
	std::shared_ptr<Core> _core;
	std::shared_ptr<Mqtt> _mqtt;
	QSettings &_settings;

	void _ring(QString number);
	void _publish(QString topic, QString message);

public:
	explicit App(QSettings &settings);
	~App();

	std::shared_ptr<Core> getCore() const;

public slots:
	void iterate();

public:
	Q_INVOKABLE void ring(QString number);

	void onGlobalStateChanged(const std::shared_ptr<linphone::Core> &lc, linphone::GlobalState gstate, const std::string &message) override;
	void onRegistrationStateChanged(const std::shared_ptr<Core> &lc, const std::shared_ptr<ProxyConfig> &cfg, RegistrationState cstate, const std::string &message) override;
	void onCallStateChanged(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, linphone::Call::State cstate, const std::string &message) override;
	void onDtmfReceived(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, int dtmf) override;
};

#endif // __APP_HPP_
