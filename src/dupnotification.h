#ifndef DUPNOTIFICATION_H
#define DUPNOTIFICATION_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include "dieklingelcontext.h"

namespace dieklingel {
    namespace dup {
        class Notification
        {
        public:
            QString toString();
            QJsonObject dataObject();
            //QJsonArray dataArray();
            dieklingel::Context context();
            bool isObject();
            //bool isArray();
            static Notification fromQJson(dieklingel::Context context, QJsonObject data);
            //static Notification fromQJson(dieklingel::Context context, QJsonArray data);
            static Notification fromString(QString message, bool secure = false);
            static Notification fromNotification(dieklingel::Context context, Notification parent);
            static Notification empty();
        private:
            Notification();
            QJsonObject m_notification;
            dieklingel::Context m_context;
            bool m_isObject;
        };
    }
}



#endif // DUPNOTIFICATION_H
