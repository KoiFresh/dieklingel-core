#ifndef __CORE_CONFIG_HPP__
#define __CORE_CONFIG_HPP__

#include <QList>
#include <QFile>
#include <QLoggingCategory>
#include <linphone++/linphone.hh>
#include <QSettings>

class CoreConfig
{
private:
	QList<QString> _paths = {
		"core.ini",
		"$HOME/.config/core.ini",
		"$HOME/core.ini",
		"/etc/dieklingel-core/core.ini",
	};
	QSettings *_settings = nullptr;

public:
	CoreConfig(/* args */);
	~CoreConfig();

	QString getConfigFilePath();

	QVariant get(QString key, const QVariant &defaultValue = QVariant());
	bool getCoreSipEnabled();
	QString getCoreSipAddress();
	QString getCoreSipPassword();

	bool getCoreMqttEnabled();
	QString getCoreMqttAddress();
	QString getCoreMqttUsername();
	QString getCoreMqttPassword();

	QMap<QString, QString> getCoreEnv();
};

#endif // __CORE_CONFIG_HPP__
