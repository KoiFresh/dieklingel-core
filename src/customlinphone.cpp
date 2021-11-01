/**
 * @decprecated
 * declared as deprecated since 2021-11-01 by Kai Mayer
 **/
#include <QTimer>
#include <QDebug>
#include <linphone/core.h>
#include <linphone/linphonecore.h>
#include <linphone/linphonecore_utils.h>
#include <bctoolbox/list.h>
#include "customlinphone.h"
#include "socket.h"

customlinphone* customlinphone::instance = nullptr;
LinphoneCore *customlinphone::lc;
LinphoneCoreVTable customlinphone::vtable = {};

customlinphone::customlinphone(QString username, QString password, QString domain, int port, QObject *parent) : QObject(parent)
{      
    instance = this;
    // linphone zyklisch iterieren
    vtable.call_state_changed = call_state_changed;
    lc = linphone_core_new(&vtable,NULL,NULL,NULL);
    proxy_cfg = linphone_proxy_config_new();
    QByteArray identity_qba = ("sip:" + username + "@" + domain + ":" + QString::number(port)).toLocal8Bit();
    from = linphone_address_new(identity_qba.data());
    QByteArray password_qba = (password).toLocal8Bit();
    info = linphone_auth_info_new(linphone_address_get_username(from),NULL,password_qba.data(),NULL,NULL,NULL);
    linphone_core_add_auth_info(lc,info);
    linphone_proxy_config_edit(proxy_cfg);
    linphone_proxy_config_set_identity(proxy_cfg,identity_qba.data()); //set identity with user name and domain
    QByteArray serverAddress_qba = (domain + ":" + QString::number(port)).toLocal8Bit();
    linphone_proxy_config_set_server_addr(proxy_cfg,serverAddress_qba.data()); // we assume domain = proxy server address
    linphone_proxy_config_enable_register(proxy_cfg,TRUE);
    linphone_core_add_proxy_config(lc,proxy_cfg); // add proxy config to linphone core
    linphone_core_set_default_proxy(lc,proxy_cfg); // set to default proxy
    linphone_proxy_config_enable_publish(proxy_cfg,true);
    linphone_proxy_config_done(proxy_cfg);

    linphone_core_set_stun_server(lc, "stun1.l.google.com:19302");

    cp = linphone_core_create_call_params(lc, lcall);
    linphone_call_params_enable_video(cp, true);
    linphone_call_params_set_video_direction(cp, LinphoneMediaDirection::LinphoneMediaDirectionSendOnly);
    linphone_core_create_conference_params(lc);

    confp = linphone_conference_params_new(lc);
    linphone_conference_params_enable_video(confp, false);

    conf = linphone_core_create_conference_with_params(lc, confp);
    //linphone_conference_ref(conf);

    //linphone_core_enter_conference(lc);

    linphone_core_set_static_picture(lc, "/home/kai/Bilder/1.jpeg");
    linphone_core_set_video_device(lc, linphone_core_get_video_devices(lc)[0]); // Set Video to Static Picture

    qDebug() << linphone_core_get_video_device(lc);

    //linphone_core_invite_with_params(lc, "vm-kai.mayer1", cp);

        //linphone_core_start_echo_calibration(lc, nullptr, nullptr, nullptr,nullptr);
        //linphone_core_enable_echo_cancellation(lc, true);
    //linphone_core_enable_echo_limiter(lc, true);

    //LinphoneConferenceParams* lcp = linphone_core_create_conference_params(lc);
    //linphone_conference_params_enable_video(lcp, true);




    timer = new QTimer(this);
    timer->setInterval(50);
    connect(timer, &QTimer::timeout, this, &customlinphone::iterate);
    timer->start();
}

customlinphone::~customlinphone()
{
    linphone_core_destroy(lc);
}

void customlinphone::iterate()
{

    linphone_core_iterate(lc);
    //linphone_call_ref(lcall);
}

void customlinphone::call(QString identity)
{
    lcall = linphone_core_get_current_call(lc);
    if((lcall &&
        linphone_call_get_state(lcall) != LinphoneCallState::LinphoneCallStreamsRunning &&
        linphone_call_get_state(lcall) != LinphoneCallState::LinphoneCallOutgoingRinging &&
        linphone_call_get_state(lcall) != LinphoneCallState::LinphoneCallOutgoingInit &&
        linphone_call_get_state(lcall) != LinphoneCallState::LinphoneCallError &&
        linphone_call_get_state(lcall) != LinphoneCallState::LinphoneCallOutgoingEarlyMedia &&
        linphone_call_get_state(lcall) != LinphoneCallOutgoingProgress) || !lcall)
    {
        m_isInOutgoingInCall = true;
        QByteArray number_qba = (identity).toLocal8Bit();

        qDebug() << "invite: " << number_qba;
        // uncommented because it is now working with
        //LinphoneCall* call = linphone_core_invite_with_params(lc, number_qba.data(),cp);


        linphone_core_create_conference_with_params(lc, confp );
        linphone_core_enter_conference(lc);
        LinphoneAddress* adress = linphone_core_create_address(lc, number_qba.data());
        const bctbx_list_t* adresses = bctbx_list_new(adress);
        qDebug() << linphone_conference_invite_participants(linphone_core_get_conference(lc), adresses, cp);


        //linphone_core_add_to_conference(lc, c);
        //linphone_call_terminate(call);
    }
}

bool customlinphone::isInIncommingCall()
{
    return m_isInIncommingCall;
}

bool customlinphone::isInOutgoingInCall()
{
    return m_isInOutgoingInCall;
}

void customlinphone::call_state_changed(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *msg){
    LinphoneCallParams *cp = nullptr;
    switch(cstate){
        case LinphoneCallIncomingReceived: // 1
            qInfo() << "Incomming Call!";
            dieklingel::socket::log("Incomming Call", "info", "linphone");


            linphone_call_accept_with_params(call, instance->cp);
            //linphone_core_add_to_conference(lc, call);
            //linphone_call_ref(call);


            /*if(linphone_call_get_state(linphone_core_get_current_call(lc)) != LinphoneCallIncomingReceived)
            {
                linphone_core_terminate_call(lc,linphone_core_get_current_call(lc));
            }
            linphone_call_accept_with_params(call, instance->cp);
            linphone_call_ref(call); */
            instance->m_isInIncommingCall = true;
            break;
        case LinphoneCallOutgoingInit: // 2
            qInfo() << "Call Outgoing Init";
             dieklingel::socket::log("Call Outgoing init", "info", "linphone");
             //linphone_core_add_to_conference(lc, call);
            instance->m_isInOutgoingInCall = true;
            break;
        case LinphoneCallOutgoingRinging: // 4
            qInfo() << "It is now ringing remotely !\n";
             dieklingel::socket::log("It is now ringing remotely", "info", "linphone");
            break;
        case LinphoneCallOutgoingEarlyMedia: // 5
            qInfo() << "Receiving some early media\n";
             dieklingel::socket::log("Receivinf some early media", "info", "linphone");
            break;
        case LinphoneCallConnected: // 6

            //linphone_core_add_to_conference(lc, call);
            qInfo() << "We are connected !\n";
            dieklingel::socket::log("We are connected", "info", "linphone");
            //linphone_call_enable_echo_cancellation(call, true);
            //linphone_call_enable_echo_cancellation(call, true);
            emit instance->callStarted();
            break;
        case LinphoneCallStreamsRunning: // 7

        qDebug() << "WE ARE IN CONF:" << linphone_core_is_in_conference(lc);
            qInfo() << "Media streams established !\n";
             dieklingel::socket::log("Media streams established", "info", "linphone");
             //linphone_call_enable_echo_limiter(call, true);
            //now  linphone_call_enable_echo_cancellation(call, true);
            linphone_core_add_to_conference(lc, call);


             //linphone_core_add_all_to_conference(lc);
        case LinphoneCallError: // 12
            qInfo() << "Call error !";
            //linphone_core_add_to_conference(lc, call);
            //linphone_core_enter_conference(lc);
             dieklingel::socket::log("Call error !", "info", "linphone");
            instance->m_isInOutgoingInCall = false;
            instance->m_isInIncommingCall = false;
            break;
        case LinphoneCallEnd: // 13
            qInfo() << "Call is terminated.\n";
            dieklingel::socket::log("Call is terminated", "info", "linphone");
            //linphone_core_leave_conference(lc);

            if(linphone_core_get_conference_size(lc) < 1)
            {

                qDebug() << "no one in conf  - leave" << linphone_core_is_in_conference(lc);
                linphone_core_leave_conference(lc);

            }
            instance->m_isInOutgoingInCall = false;
            instance->m_isInIncommingCall = false;
            emit instance->callFinished();
            //linphone_call_unref(call);
            //call = nullptr;
            break;
        default:
            qInfo() << "Unhandled notification " << cstate;
            dieklingel::socket::log(QString() + "unhandled notification -> " + msg, "info", "linphone");
    }
}
