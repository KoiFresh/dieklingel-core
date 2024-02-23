#ifndef __APP_HPP__
#define __APP_HPP__

#include <linphone++/linphone.hh>
#include <QLoggingCategory>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>

#include "Capturer.hpp"
#include "CoreConfig.hpp"
#include "InactivityDetector.hpp"
#include "Mqtt.hpp"
#include "video/SplitterCamera.hpp"

using namespace linphone;

class App : public QObject, public CoreListener
{
	Q_OBJECT
private:
	int &_argc;
	char **_argv;
	CoreConfig &_settings;
	std::shared_ptr<Core> _core;
	std::shared_ptr<Mqtt> _mqtt;
	std::shared_ptr<QCoreApplication> _application;
	std::shared_ptr<QQmlApplicationEngine> _engine;
	std::shared_ptr<InactivityDetector> _filter;
	std::shared_ptr<Capturer> _capturer;
	// std::shared_ptr<Camera> _camera;

	void _initEnv();
	void _initCapturer();
	void _initApplication();
	void _initCore();
	void _initCamera();
	void _initMqtt();

	void _ring(QString number);
	void _iterate();

public:
	explicit App(int &argc, char **argv, CoreConfig &settings);
	~App();

	QString getVersion();

	int exec();
	Q_INVOKABLE void ring(QString number);
	Q_INVOKABLE void publish(QString topic, QString message);
	Q_INVOKABLE void snapshot();
	Q_INVOKABLE QString env(QString key);

	void printCoreInformation();

signals:
	void inactivity();
	void messageReceived(QString topic, QString message);
	void snapshotTaken(QByteArray snapshot);

protected:
	void onGlobalStateChanged(const std::shared_ptr<linphone::Core> &lc, linphone::GlobalState gstate, const std::string &message) override;
	void onRegistrationStateChanged(const std::shared_ptr<Core> &lc, const std::shared_ptr<ProxyConfig> &cfg, RegistrationState cstate, const std::string &message) override;
	void onCallStateChanged(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, linphone::Call::State cstate, const std::string &message) override;
	void onDtmfReceived(const std::shared_ptr<linphone::Core> &lc, const std::shared_ptr<linphone::Call> &call, int dtmf) override;
};

#endif // __APP_HPP_
