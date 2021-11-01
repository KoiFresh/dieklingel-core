#ifndef SIPCLIENT_H
#define SIPCLIENT_H

#include <QtDebug>
#include <QString>
#include <QTimer>
#include <linphone/core.h>
#include "sipuri.h"

namespace sip
{
    enum SessionState {
        Ended = 0,
        Active = 1,
        Waiting = 2
    };

    class Client : public QObject
    {
        Q_OBJECT
    public:
        static void Register(QString username, QString password, QString domain, int port = 5060, bool isDefaultConfig = true);
        static void Call(sip::Uri sipuri);
        static void Call(QList<Uri*> uris);
        static Client *GetInstance();
    signals:
        void SessionStateChanged(SessionState state);
    private:
        Client() {};
        static void m_initialize();
        static void m_callCreated(LinphoneCore *core, LinphoneCall *call);
        static void m_callStateChanged(LinphoneCore *core, LinphoneCall *call, LinphoneCallState callState, const char *message);
        static QTimer *m_iteration_timer;
        static LinphoneCoreVTable m_vtable;
        static LinphoneCore *m_linphoneCore;
        static LinphoneCall *m_linphoneCall;
        static LinphoneCallParams *m_linhponeCallParams;
        static LinphoneProxyConfig *m_linphoneProxyConfig;
        static LinphoneConference *m_linphoneConference;
        static LinphoneConferenceParams *m_linphoneConferenceParams;
        static LinphoneAuthInfo *m_authInfo;
        static Client *m_instance;
    private slots:
        static void m_iterate();
        static void m_sessionStateChanged(sip::SessionState state);
    };
}



#endif // SIPCLIENT_H
