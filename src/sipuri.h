#ifndef SIPURI_H
#define SIPURI_H

#include <QString>

namespace sip
{
    class Uri
    {
    public:
        Uri(QString username, QString domain, int port = 5060);
        Uri(QString uri);
        QString toString();
        QByteArray toQByteArray();
    private:
        QString m_username;
        QString m_domain;
        int m_port;
    };
}

#endif // SIPURI_H
