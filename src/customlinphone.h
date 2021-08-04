#ifndef CUSTOMLINPHONE_H
#define CUSTOMLINPHONE_H

#include <QObject>
#include <QTimer>
#include <linphone/core.h>

/*
 *  customlinphone.h
 *  Beinhaltet Liblinphone. Konfiguriert Linphone in einem easy-to-use modus und stellt Schnittstellen passend
 *  zum backend bereit.
 */


class customlinphone : public QObject
{
    Q_OBJECT
public:
    explicit customlinphone(QString username, QString password, QString domain, int port,QObject *parent = nullptr);
    ~customlinphone();
    void call(QString identity);
    bool isInOutgoingInCall();
    bool isInIncommingCall();

signals:
    void callStarted();
    void callFinished();

private:
    void iterate();
    QTimer* timer;
    LinphoneCoreVTable vtable = {};
    LinphoneCore *lc;
    LinphoneCall *lcall = NULL;
    LinphoneCallParams *cp;
    LinphoneProxyConfig* proxy_cfg;
    LinphoneAddress *from;
    LinphoneAuthInfo *info;
    bool m_isInOutgoingInCall;
    bool m_isInIncommingCall;
    static customlinphone* instance;
    static void call_state_changed(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *msg);
    static void registration_state_changed(struct _LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message);
};

#endif // CUSTOMLINPHONE_H
