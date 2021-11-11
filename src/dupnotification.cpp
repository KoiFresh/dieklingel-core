#include <QJsonDocument>
#include <QDebug>
#include "dupnotification.h"
#include "dieklingelcontext.h"

dieklingel::dup::Notification::Notification()
{

}

dieklingel::dup::Notification dieklingel::dup::Notification::fromString(QString message, bool secure)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc.object();
    QJsonObject data = obj["Body"].toObject()["Data"].toObject();
    Context context = dieklingel::QStringAsContext(obj["Body"].toObject()["Context"].toString());

    context = (secure && context == Context::Unlock) ? Context::SecureUnlock : context;
    context = (!secure && context == Context::SecureUnlock) ? Context::Unlock : context;

    return Notification::fromQJson(context, data);
}

dieklingel::dup::Notification dieklingel::dup::Notification::fromQJson(dieklingel::Context context, QJsonObject data)
{
    QJsonObject obj;
    QJsonObject header;
    obj.insert("Header", header);
    QJsonObject body;
    body.insert("Context", dieklingel::ContextAsQString(context));
    body.insert("Data", data);
    obj.insert("Body", body);
    Notification notification;
    notification.m_notification = obj;
    notification.m_context = context;
    notification.m_isObject = true;
    return  notification;
}

/*dieklingel::dup::Notification dieklingel::dup::Notification::fromQJson(dieklingel::Context context, QJsonArray data)
{
    QJsonObject obj;
    QJsonObject header;
    obj.insert("Header", header);
    QJsonObject body;
    body.insert("Context", dieklingel::ContextAsQString(context));
    body.insert("Data", data);
    obj.insert("Body", body);
    Notification notification;
    notification.m_notification = obj;
    notification.m_context = context;
    notification.m_isObject = false;
    return  notification;
}*/

dieklingel::dup::Notification dieklingel::dup::Notification::fromNotification(dieklingel::Context context, Notification parent)
{
    /*Notification notification;
    if(parent.isObject())
    {
        notification = Notification::fromQJson(context, parent.dataObject());
    }else
    if(parent.isArray())
    {
        notification = Notification::fromQJson(context, parent.dataArray());
    }
    return  notification;
    */
   return parent;
}

dieklingel::dup::Notification dieklingel::dup::Notification::empty()
{
    return Notification();
}

QString dieklingel::dup::Notification::toString()
{
    QJsonDocument doc(m_notification);
    return doc.toJson();
}

QJsonObject dieklingel::dup::Notification::dataObject()
{
    if(m_isObject)
    {
        return m_notification["Body"].toObject()["Data"].toObject();
    }else
    {
        return QJsonObject();
    }
}

/*QJsonArray dieklingel::dup::Notification::dataArray()
{
    if(!m_isObject)
    {
        return m_notification["Body"].toObject()["Data"].toArray();
    }else
    {
        return QJsonArray();
    }
}*/

dieklingel::Context dieklingel::dup::Notification::context()
{
    return m_context;
}

bool dieklingel::dup::Notification::isObject()
{
    return m_isObject;
}

/*bool dieklingel::dup::Notification::isArray()
{
    return !m_isObject;
}*/
