#include <iostream>
#include <QDir>
#include <linphone++/linphone.hh>
#include <unistd.h>

#include "listeners/CoreCallback.hpp"

using namespace linphone;

int main(int argc, char *argv[])
{
    qInfo() << "-----------------------";
    qInfo() << "|   dieKlingel Core   |";
    qInfo() << "-----------------------";

    auto factory = Factory::get();
    auto path = QDir::currentPath().toStdString();
    factory->setDataResourcesDir(path);
    factory->setImageResourcesDir(path);
    factory->setTopResourcesDir(path);

    qInfo() << "wd:" << path.c_str();
    qInfo() << "config:" << factory->getConfigDir(nullptr).c_str();
    qInfo() << "data:" << factory->getDataDir(nullptr).c_str();
    qInfo() << "ressources:" << factory->getDataResourcesDir().c_str();

    auto core = factory->createCore("", "", nullptr);

    // TODO: read from config file
    auto address = factory->createAddress("sip:koifresh@sip.linphone.org");

    auto info = factory->createAuthInfo(address->getUsername(), "", "PASSWORD", "", "", "");
    core->addAuthInfo(info);

    auto proxy = core->createProxyConfig();
    proxy->edit();
    proxy->setIdentityAddress(address);
    proxy->setServerAddr(address->getDomain());
    proxy->enableRegister(true);
    proxy->done();
    core->addProxyConfig(proxy);
    core->setDefaultProxyConfig(proxy);

    core->enableSelfView(false);
    core->enableVideoCapture(false);
    core->enableVideoDisplay(false);

    auto nat = core->createNatPolicy();
    nat->enableStun(true);
    nat->setStunServer("stun.l.google.com:19302");
    nat->enableIce(true);
    core->setNatPolicy(nat);

    auto coreListener = std::shared_ptr<CoreListener>(new CoreCallback());
    core->addListener(coreListener);

    auto state = core->start();
    if (state != 0)
    {
        qCritical() << "the linphone core could not be started successfully!";
        exit(state);
    }

    while (true)
    {
        core->iterate();
        usleep(50000);
    }

    return 0;
}
