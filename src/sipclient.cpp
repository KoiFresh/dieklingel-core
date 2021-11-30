#include <QTimer>
#include <linphone/core.h>
#include <linphone/call.h>
#include <linphone/callbacks.h>
#include <linphone/linphonecore.h>
#include <linphone/linphonecore_utils.h>
#include <linphone/payload_type.h>
#include "sipclient.h"
#include "toolbox.h"

/**
 * This is a class, which represents every sip connection. It easyli wrapps the linphone core into a simple to use class for
 * the specific use case
 *
 * @author Kai Mayer
 * @version 1.0
 *
 */

#define LINPHONE_CORE_PLAYBACK_GAIN_SILENT 100
#define LINPHONE_CORE_PLAYBACK_GAIN_LOUD 1

/**
 * annotaion by the Author of this file:
 * In case that a function is called before the Core was Initialized, we initialize the core.
 * for future use we check if already initialized through the m_linphoneCore Object. If this is a nullprt
 * we are still not initialized and  should call m_initialize()
 */

LinphoneCoreVTable sip::Client::m_vtable = {};
LinphoneCore *sip::Client::m_linphoneCore;
LinphoneCall *sip::Client::m_linphoneCall = nullptr;
LinphoneCallParams *sip::Client::m_linhponeCallParams = nullptr;
LinphoneProxyConfig *sip::Client::m_linphoneProxyConfig = nullptr;
LinphoneConference *sip::Client::m_linphoneConference = nullptr;
LinphoneConferenceParams *sip::Client::m_linphoneConferenceParams = nullptr;
QTimer *sip::Client::m_iteration_timer = nullptr;
sip::Client *sip::Client::m_instance = nullptr;
PayloadType* sip::Client::m_payloadType = nullptr;
bctbx_list_t* sip::Client::m_videoPayloadTypes = nullptr;

sip::Client *sip::Client::GetInstance()
{
    if(nullptr == m_linphoneCore)
    {
        m_initialize();
    }
    return m_instance;
}

/**
 * @brief sip::Client::m_initialize
 * Initializes the static objects
 */
void sip::Client::m_initialize()
{
    linphone_core_set_log_level(OrtpLogLevel::ORTP_ERROR);
    if(nullptr == m_linphoneCore)
    {
#if DEBUG
        qDebug() << "[DEBUG][sipclient.cpp, m_initialize()] \r\n\t starting to initialize the Linphone Core and the iteration timer";
#endif
        m_payloadType = &payload_type_vp8;
        m_videoPayloadTypes = bctbx_list_new(m_payloadType);
        m_vtable.call_state_changed = m_callStateChanged;
        m_vtable.call_created = m_callCreated;
        m_linphoneCore = linphone_core_new(&m_vtable, NULL, NULL, NULL);
        linphone_core_set_stun_server(m_linphoneCore, "stun1.l.google.com:19302");
        //linphone_core_set_video_codecs(m_linphoneCore, m_videoPayloadTypes);
        //linphone_core_enable_payload_type(m_linphoneCore, m_payloadType, true);
        linphone_core_set_preferred_framerate(m_linphoneCore, 26.0);
        //linphone_core_set_video_device(m_linphoneCore, linphone_core_get_video_devices(m_linphoneCore)[2]);
        //qDebug() << linphone_core_get_video_devices(m_linphoneCore)[0];
#if DEBUG
        qDebug() << "[DEBUG][sipclient.cpp, m_initialize()] \r\n\t " << "Linphone Core Version:" << linphone_core_get_version();
#endif
        //qDebug() << linphone_core_get_ringback(m_linphoneCore);
        /*linphone_core_set_ringback(m_linphoneCore, NULL);
        linphone_core_set_ring(m_linphoneCore ,NULL);
        linphone_core_set_ring_during_incoming_early_media(m_linphoneCore, false);
        linphone_core_set_ringer_device(m_linphoneCore, NULL);*/
        m_instance = new Client();
        m_iteration_timer = new QTimer();
        m_iteration_timer->setInterval(500);
        // cbot connect(m_iteration_timer, &QTimer::timeout, Client::m_iterate);
        // cbot m_iteration_timer->start();
        connect(GetInstance(), &sip::Client::SessionStateChanged, sip::Client::m_sessionStateChanged);
#if DEBUG
        qDebug() << "[DEBUG][sipclient.cpp, m_initialize()] \r\n\t end of initialize Linphone Core";
#endif
    }
}

/**
 * @brief sip::Client::m_iterate
 * Called every 50 milliseconds to iterate the linphonecore
 */
void sip::Client::Iterate()
{
    linphone_core_iterate(m_linphoneCore);
}
/*void sip::Client::m_iterate()
{
    linphone_core_iterate(m_linphoneCore);
}*/

void sip::Client::m_sessionStateChanged(sip::SessionState state)
{
    switch (state) {
            //linphone_core_set_playback_gain_db(m_linphoneCore, LINPHONE_CORE_PLAYBACK_GAIN_SILENT);
            break;
        case sip::SessionState::Active:
            //linphone_core_set_playback_gain_db(m_linphoneCore, LINPHONE_CORE_PLAYBACK_GAIN_LOUD);
            break;
        case sip::SessionState::Ended:
            //linphone_core_set_playback_gain_db(m_linphoneCore, LINPHONE_CORE_PLAYBACK_GAIN_SILENT);
            break;
    }
}

/**
 * @brief sip::Client::m_callStateChanged
 * @param core
 * @param call
 * @param callState
 * @param message
 * Callback for the LinphoneCorec call_state_changed function
 */

void sip::Client::m_callCreated(LinphoneCore *core, LinphoneCall *call)
{
    // notihg to do after a call is created
}

void sip::Client::m_callStateChanged(LinphoneCore *core, LinphoneCall *call, LinphoneCallState callState, const char *message)
{
    const bctbx_list_t *client = NULL;
    LinphoneCall *current;
    switch(callState){
        case LinphoneCallIncomingReceived: // 1
            qInfo() << "[INFO][sipclient.cpp, m_callStateChanged] Incomming Call!";
            if(linphone_core_in_call(core))
            {
                current = linphone_core_get_current_call(core);
                if(linphone_call_get_state(current) == LinphoneCallOutgoingEarlyMedia)
                {
                    linphone_call_terminate(current);
                }
            }

            if(linphone_core_get_calls_nb(core) > 1)  // new
            { // new
                //*linphone_core_add_to_conference(core, call);
                //*linphone_call_accept_with_params(call, m_linhponeCallParams);
                //*linphone_core_add_all_to_conference(core);
                linphone_core_decline_call(core,call, LinphoneReason::LinphoneReasonBusy);
            } else {// new
                linphone_call_accept_with_params(call, m_linhponeCallParams);
            }

            break;
        case LinphoneCallOutgoingInit: // 2
            qInfo() << "[INFO][sipclient.cpp, m_callStateChanged] Call Outgoing Init";
            break;
       case LinphoneCallOutgoingRinging: // 4
            qInfo() << "[INFO][sipclient.cpp, m_callStateChanged] It is now ringing remotely !";

            break;
       case LinphoneCallOutgoingEarlyMedia: // 5
            qInfo() << "[INFO][sipclient.cpp, m_callStateChanged] Receiving some early media";
            break;
       case LinphoneCallConnected: // 6
            qInfo() << "[INFO][sipclient.cpp, m_callStateChanged] We are connected !";
            //linphone_core_add_to_conference(core, call);
            linphone_core_set_playback_gain_db(m_linphoneCore, 1.0f);

            break;
       case LinphoneCallStreamsRunning: // 7
            qInfo() << "[INFO][sipclient.cpp, m_callStateChanged] Media streams established !";

            //linphone_core_enter_conference(core);

            emit m_instance->SessionStateChanged(SessionState::Active);
            if(linphone_core_get_calls_nb(core) > 1)
            {
                //* linphone_core_add_all_to_conference(core);
            }
            if(linphone_core_get_conference_size(core) > 1)  // new
            { // new
                if(!linphone_call_is_in_conference(call))
                {
                    //*linphone_core_add_to_conference(core, call);
                }
            } // new
            client = linphone_core_get_calls(core);
            while(NULL != client)
            {
                LinphoneCall *cl = static_cast<LinphoneCall*>(client->data);
                if(linphone_call_get_state(cl) == LinphoneCallState::LinphoneCallOutgoingEarlyMedia)
                {
                    linphone_call_terminate(cl);
                }
                client = client->next;
            }
            //linphone_core_enter_conference(core);
        case LinphoneCallError: // 12
            //linphone_core_set_playback_gain_db(m_linphoneCore, 1);
            qInfo() << "[INFO][sipclient.cpp, m_callStateChanged] Call error !";
            break;
        case LinphoneCallEnd: // 13
            qInfo() << "[INFO][sipclient.cpp, m_callStateChanged] Call is terminated.";
            if(linphone_core_get_conference_size(core) < 1)
            {
                emit m_instance->SessionStateChanged(SessionState::Ended);
            }
            break;
        default:
            qInfo() << "[INFO][sipclient.cpp, m_callStateChanged] Unhandled notification " << callState;
     }
}

/**
 * @brief sip::Client::Register
 * @param username username for the sip server
 * @param password password for the sip server
 * @param domain domain of the sip server
 * @param port port on wich the sip server is listening
 * @param asDefault false if the given account should not be used as default
 */
void sip::Client::Register(QString username, QString password, QString domain, int port, bool isDefaultConfig)
{
    if(nullptr == m_linphoneCore)
    {
        m_initialize();
    }
#if DEBUG
    qDebug() << "[DEBUG][sipclient.cpp, Register()] \r\n\t starting to register new account";
#endif
    Uri *uri = new Uri(username, domain, port);
    LinphoneAuthInfo *info = linphone_auth_info_new(username.toLocal8Bit().data() , NULL, password.toLocal8Bit().data(), NULL, NULL, NULL);
    linphone_core_add_auth_info(m_linphoneCore, info);
    LinphoneProxyConfig *proxyCfg = linphone_proxy_config_new();
    /*LinphoneProxyConfig *proxyCfg = linphone_core_create_proxy_config(m_linphoneCore);
*/
    linphone_proxy_config_edit(proxyCfg);
    linphone_proxy_config_set_identity(proxyCfg, dieklingel::Toolbox::QString_to_Char(uri->toString()));
    /*LinphoneAddress *addr = linphone_address_new( dieklingel::Toolbox::QString_to_Char(uri->toString()));
    linphone_proxy_config_set_identity_address(proxyCfg,addr);
*/
    linphone_proxy_config_set_server_addr(proxyCfg, dieklingel::Toolbox::QString_to_Char(domain + ":" + QString::number(port)));
    linphone_proxy_config_enable_register(proxyCfg, true);
    linphone_proxy_config_enable_publish(proxyCfg, true);
    linphone_core_add_proxy_config(m_linphoneCore, proxyCfg);
    linphone_proxy_config_done(proxyCfg);
    if(isDefaultConfig)
    {
        linphone_core_set_default_proxy_config(m_linphoneCore, proxyCfg);
#ifdef DEBUG
        qDebug() << "[DEBUG][sipclient.cpp, Register()] \r\n\t register the account as default";
#endif
    }
    m_linhponeCallParams = linphone_core_create_call_params(m_linphoneCore, m_linphoneCall);
    //linphone_call_params_enable_video(m_linhponeCallParams, true);
    linphone_call_params_enable_video(m_linhponeCallParams, false);
    linphone_call_params_set_video_direction(m_linhponeCallParams, LinphoneMediaDirection::LinphoneMediaDirectionSendOnly);
    m_linphoneConferenceParams = linphone_conference_params_new(m_linphoneCore);
    linphone_conference_params_enable_video(m_linphoneConferenceParams, true);
    if(nullptr == m_linphoneConference)
    {
        //m_linphoneConference = linphone_core_create_conference_with_params(m_linphoneCore, m_linphoneConferenceParams);
    }
#ifdef DEBUG
   qDebug() << "[DEBUG][sipclient.cpp, Register()] \r\n\t end of registering a new account";
#endif
}

/**
 * @brief sip::Client::Call
 * Invites one sipuri, if there is already a call, add both to a conference
 * @param sipuri uri to invite for example <sip:username@sipserver:port>
 */
void sip::Client::Call(sip::Uri sipuri)
{
    QList<Uri*> uris;
    uris[0] = &sipuri;
    Call(uris);
}

/**
 * @brief sip::Client::Call
 * Invites a number of uris to a conference
 * @param sipuris an array of uris to invite for example <sip:username@sipserver:port>
 *
 */
void sip::Client::Call(QList<sip::Uri*> uris)
{
    if(nullptr == m_linphoneCore)
    {
        m_initialize();
    }
    /**
     * @todo check if at least one proxy is registerd
     */
    if(!linphone_core_in_call(m_linphoneCore))
    {
#ifdef DEBUG
        qDebug() << "[DEBUG][sipclient.cpp, Call()] \r\n\t start to add a call to a conference";
#endif
        //linphone_core_enter_conference(m_linphoneCore);
        /**
         * @todo fix the linphone_conference_invite_participants method this should only be called once
         * after the loop, and the sip uri of every participant should be added to the bctbx_list_t.
         * But I dont get the type of bctbx_list_t and it doesnt work to add more then one participant
         * so i choose the faster way and call each participant by an extra invite :(
         * @author Kai Mayer
         */
        // only invite new participants if there is no active call

        if(linphone_core_get_conference_size(m_linphoneCore) < 2)
        {
            /*bctbx_list_t *start;
            LinphoneAddress *address = linphone_core_create_address(m_linphoneCore, uris[0]->toQByteArray().data());
            start = bctbx_list_new(address);
            for(int i = 1; i < uris.length(); i++)
            {
                LinphoneAddress *address = linphone_core_create_address(m_linphoneCore, uris[i]->toQByteArray().data());
                start = bctbx_list_append(start, address);
            }
            linphone_conference_invite_participants(m_linphoneConference,start, m_linhponeCallParams); */

            /*for(int i = 0; i < uris.length(); i++)
            {
                LinphoneAddress *address = linphone_core_create_address(m_linphoneCore, uris[i]->toQByteArray().data());
                bctbx_list_t *l = bctbx_list_new(address);
                //linphone_conference_invite_participants(m_linphoneConference,l, m_linhponeCallParams);
                LinphoneCall *cl = linphone_core_invite_address_with_params(m_linphoneCore, address, m_linhponeCallParams);
                //linphone_call_set_output_audio_device(cl, nullptr);
                linphone_core_set_playback_gain_db(m_linphoneCore, 1000.0f);
            } */
            if(uris.length() > 0) 
            {
                LinphoneAddress *address = linphone_core_create_address(m_linphoneCore, uris[0]->toQByteArray().data());
                LinphoneCall *cl = linphone_core_invite_address_with_params(m_linphoneCore, address, m_linhponeCallParams);
                linphone_core_set_playback_gain_db(m_linphoneCore, 1000.0f);
                emit m_instance->SessionStateChanged(SessionState::Waiting);
            }
        }
    }else
    {
#ifdef DEBUG
        qDebug() << "[DEBUG][sipclient.cpp, Call()] \r\n\t cant initialize the call, because we are already in a call";
#endif
    }
}
