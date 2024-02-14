#ifndef __MQTT_HPP__
#define __MQTT_HPP__

#include <mqtt/client.h>
#include <QLoggingCategory>
#include <QtConcurrent>
#include <QString>

class Mqtt : public QObject, public mqtt::callback
{
	Q_OBJECT
private:
	QString _address;
	QString _username;
	QString _password;
	QStringList _subscriptions;
	std::shared_ptr<mqtt::async_client> _mqtt;

public:
	Mqtt(QString address);
	~Mqtt();

	void connect(QString username, QString password);
	void subscribe(QString topic);
	void publish(QString topic, QString message);

	void connected(const std::string &cause) override;
	void connection_lost(const std::string &cause) override;
	void message_arrived(mqtt::const_message_ptr message) override;

signals:
	void messageReceived(QString topic, QString message);
};

#endif // __MQTT_HPP__
