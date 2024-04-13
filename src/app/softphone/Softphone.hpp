#ifndef __SOFTPHONE_SOFTPHONE_HPP__
#define __SOFTPHONE_SOFTPHONE_HPP__

#include <QDebug>
#include <QDir>
#include <QTimer>
#include <linphone++/linphone.hh>

#include "../audio/Audio.hpp"
#include "../camera/Camera.hpp"
#include "../setup/Configuration.hpp"
#include "../setup/Setup.hpp"

class Softphone : public Configuration,
                  public linphone::CoreListener,
                  public std::enable_shared_from_this<Softphone> {
    Q_OBJECT
   private:
    QTimer _timer;
    std::shared_ptr<linphone::Core> _core;
    bool _isSetupCompleted = false;
    QString _username;
    QString _password;
    QString _proxy;
    Core::Setup *_setup;

   private slots:
    void _iterate();

   protected:
    void onGlobalStateChanged(
        const std::shared_ptr<linphone::Core> &lc,
        linphone::GlobalState gstate,
        const std::string &message
    ) override;

    void onRegistrationStateChanged(
        const std::shared_ptr<linphone::Core> &lc,
        const std::shared_ptr<linphone::ProxyConfig> &cfg,
        linphone::RegistrationState cstate,
        const std::string &message
    ) override;

    void onCallStateChanged(
        const std::shared_ptr<linphone::Core> &lc,
        const std::shared_ptr<linphone::Call> &call,
        linphone::Call::State cstate,
        const std::string &message
    ) override;

    void onDtmfReceived(
        const std::shared_ptr<linphone::Core> &lc,
        const std::shared_ptr<linphone::Call> &call,
        int dtmf
    ) override;

   public:
    Softphone(Core::Setup *setup, std::shared_ptr<linphone::Core> core);
    ~Softphone();

    Q_INVOKABLE void auth(QString username, QString password);
    Q_INVOKABLE void proxy(QString proxy);
    Q_INVOKABLE void transport(QStringList transports);

    Q_INVOKABLE void call(QString number);

    void onSetupCompleted() override;
    void print(QTextStream &stream) override;
};

#endif  // __SOFTPHONE_SOFTPHONE_HPP__
