#include "CoreConfig.hpp"

CoreConfig::CoreConfig() {
    for (QString path : _paths) {
        if (QFile::exists(path)) {
            this->_settings = new QSettings(path, QSettings::Format::IniFormat);
            break;
        }
    }
}

CoreConfig::~CoreConfig() {
    if (this->_settings == nullptr) {
        return;
    }
    delete this->_settings;
}

QString CoreConfig::getConfigFilePath() {
    if (this->_settings == nullptr) {
        return "N/A";
    }
    return this->_settings->fileName();
}

QVariant CoreConfig::get(QString key, const QVariant &defaultValue) {
    if (this->_settings == nullptr) {
        return defaultValue;
    }
    return this->_settings->value(key, defaultValue);
}

bool CoreConfig::getCoreQmlEnabled() {
    return this->get("core.qml/enabled", QVariant(false)).toBool();
}

QUrl CoreConfig::getCoreQmlEntry() {
    return QUrl(this->get("core.qml/entry", QVariant("qml/main.qml")).toString()
    );
}

bool CoreConfig::getCoreSipEnabled() {
    return this->get("core.sip/enabled", QVariant(false)).toBool();
}

QString CoreConfig::getCoreSipAddress() {
    return this->get("core.sip/address").toString();
}

QString CoreConfig::getCoreSipPassword() {
    return this->get("core.sip/password").toString();
}

QString CoreConfig::getCoreSipCaptureDevice() {
    return this->get("core.sip/capture-device", QVariant("h")).toString();
}

QString CoreConfig::getCoreSipPlaybackDevice() {
    return this->get("core.sip/playback-device").toString();
}

QStringList CoreConfig::getCoreSipTransports() {
    return this->get("core.sip/transports", QVariant("tcp, tls"))
        .toStringList();
}

bool CoreConfig::getCoreMqttEnabled() {
    return this->get("core.mqtt/enabled", QVariant(false)).toBool();
}

QString CoreConfig::getCoreMqttAddress() {
    return this->get("core.mqtt/address").toString();
}

QString CoreConfig::getCoreMqttUsername() {
    return this->get("core.mqtt/username").toString();
}

QString CoreConfig::getCoreMqttPassword() {
    return this->get("core.mqtt/password").toString();
}

QStringList CoreConfig::getCoreMqttSubscriptions() {
    return this->get("core.mqtt/subscriptions").toStringList();
}

QMap<QString, QString> CoreConfig::getCoreEnv() {
    QMap<QString, QString> env;
    if (this->_settings == nullptr) {
        return env;
    }

    this->_settings->beginGroup("core.env");
    for (const QString &key : this->_settings->childKeys()) {
        env.insert(key, this->_settings->value(key).toString());
    }
    this->_settings->endGroup();

    return env;
}
