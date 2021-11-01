#include "sipuri.h"
#include <QDebug>

/**
 * @brief sip::Uri::Uri
 * @param username the username on the sipserver to depict
 * @param domain the domain of the sipserver
 * @param port the port of the sipserver
 */
sip::Uri::Uri(QString username, QString domain, int port)
{
    m_username = username;
    m_domain = domain;
    m_port = port;
}

sip::Uri::Uri(QString uri) {
    m_username = uri.mid(0, uri.indexOf("@"));
    m_domain = uri.mid(uri.indexOf("@") + 1, uri.indexOf(":") - 1 - uri.indexOf("@"));
    m_port = uri.mid(uri.indexOf(":") + 1, uri.length() - 1 - uri.indexOf(":")).toUInt();
}

/**
 * @brief sip::Uri::toString
 * @return returns the sip uri as string format
 */
QString sip::Uri::toString()
{
    return "sip:" + m_username + "@" + m_domain + ":" + QString::number(m_port);
}

/**
 * @brief sip::Uri::toString
 * @return returns the sip uri as qbytearray
 */
QByteArray sip::Uri::toQByteArray()
{
    return toString().toLocal8Bit();
}
