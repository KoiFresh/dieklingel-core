#ifndef SYSTEM_H
#define SYSTEM_H

#include <QJsonArray>
#include <QSettings>

namespace dieklingel {
    class System
    {
    public:
        static void writeDevices(QJsonArray array);
        static QJsonArray readDevices();
        static void setDeviceConfigFilePath(QString path);
        static void writeUsers(QJsonArray array);
        static QJsonArray readUsers();
        static void setUserConfigFilePath(QString path);
        static void execute(QString event);
        static void execute(QString event, QStringList args);
        static QString hashPow(QString value, int power = 1);
    private:
        System();
        static QString m_deviceConfigFilePath;
        static QString m_userConfigFilePath;
        static void m_writeFile(QString path, QString content);
        static QString m_readFile(QString path);
    };
}


#endif // SYSTEM_H
