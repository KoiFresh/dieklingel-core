#include "CoreConfig.hpp"

CoreConfig::CoreConfig()
{
	for (QString path : _paths)
	{
		if (QFile::exists(path))
		{
			this->_settings = new QSettings(path, QSettings::Format::IniFormat);
			break;
		}
	}
}

CoreConfig::~CoreConfig()
{
	if (this->_settings == nullptr)
	{
		return;
	}
	delete this->_settings;
}

QString CoreConfig::getConfigFilePath()
{
	if (this->_settings == nullptr)
	{
		return "N/A";
	}
	return this->_settings->fileName();
}

QVariant CoreConfig::get(QString key, const QVariant &defaultValue)
{
	if (this->_settings == nullptr)
	{
		return defaultValue;
	}
	return this->_settings->value(key, defaultValue);
}

QUrl CoreConfig::getCoreQmlEntry()
{
	return QUrl(this->get("core.qml/entry", QVariant("qml/main.qml")).toString());
}

bool CoreConfig::getCoreSipEnabled()
{
	return this->get("core.sip/enabled", QVariant(false)).toBool();
}

QString CoreConfig::getCoreSipAddress()
{
	return this->get("core.sip/address").toString();
}

QString CoreConfig::getCoreSipPassword()
{
	return this->get("core.sip/password").toString();
}

bool CoreConfig::getCoreMqttEnabled()
{
	return this->get("core.mqtt/enabled", QVariant(false)).toBool();
}

QString CoreConfig::getCoreMqttAddress()
{
	return this->get("core.mqtt/address").toString();
}

QString CoreConfig::getCoreMqttUsername()
{
	return this->get("core.mqtt/username").toString();
}

QString CoreConfig::getCoreMqttPassword()
{
	return this->get("core.mqtt/password").toString();
}

QMap<QString, QString> CoreConfig::getCoreEnv()
{
	QMap<QString, QString> env;
	if (this->_settings == nullptr)
	{
		return env;
	}

	this->_settings->beginGroup("core.env");
	for (const QString &key : this->_settings->childKeys())
	{
		env.insert(key, this->_settings->value(key).toString());
	}
	this->_settings->endGroup();

	return env;
}
