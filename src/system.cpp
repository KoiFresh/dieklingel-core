#include "system.h"
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QObject>
#include <QCryptographicHash>

QString dieklingel::System::m_deviceConfigFilePath = "/etc/dieklingel/devices.json";
QString dieklingel::System::m_userConfigFilePath = "/etc/dieklingel/users.json";

dieklingel::System::System()
{

}

void dieklingel::System::execute(QString event, QStringList args)
{
    QString path = "/etc/dieklingel/scripts/on-" + event + ".sh";
    QProcess* p = new QProcess();
    p->setProgram(path);
    p->setArguments(args);
    p->start();
#if DEBUG
    QObject::connect(p, &QProcess::errorOccurred,[p](){
        qDebug() << "[DEBUG][system.cpp, execute()] \r\n\t " << p->errorString();
    });
#endif

}

void dieklingel::System::execute(QString event)
{
    execute(event, QStringList());
}

QString dieklingel::System::hashPow(QString value, int power)
{
    QByteArray v = value.toLocal8Bit();
    for(int i = 0; i < power; i++)
    {
        QByteArray a = v;
        v = QCryptographicHash::hash(a, QCryptographicHash::Sha256).toHex();
    }
    return  v;
}

QJsonArray dieklingel::System::readDevices()
{
    QString content = m_readFile(m_deviceConfigFilePath);
    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());
    return document.array();
}

void dieklingel::System::writeDevices(QJsonArray array)
{
    QJsonDocument document(array);
    m_writeFile(m_deviceConfigFilePath, document.toJson());
}

void dieklingel::System::setDeviceConfigFilePath(QString path)
{
    m_deviceConfigFilePath = path;
}

QJsonArray dieklingel::System::readUsers()
{
    QString content = m_readFile(m_userConfigFilePath);
    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());
    return document.array();
}

void dieklingel::System::writeUsers(QJsonArray array)
{
    QJsonDocument document(array);
    m_writeFile(m_userConfigFilePath, document.toJson());
}

void dieklingel::System::setUserConfigFilePath(QString path)
{
    m_userConfigFilePath = path;
}

void dieklingel::System::m_writeFile(QString path, QString content)
{
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate);
    file.write(content.toUtf8());
    file.close();
}

QString dieklingel::System::m_readFile(QString path)
{
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file.readAll();
    file.close();
    return content;
}
