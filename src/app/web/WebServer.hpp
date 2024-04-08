#ifndef __WEB_WEBSERVER_HPP__
#define __WEB_WEBSERVER_HPP__

#include <QDebug>
#include <QObject>

#include "../setup/Configuration.hpp"

class WebServer : public Configuration {
    Q_OBJECT
   private:
    int _port = 80;

   public:
    WebServer();
    ~WebServer();

    void onSetupCompleted() override;
    void print(QTextStream &log) override;

    Q_INVOKABLE void port(int value);
};

#endif  // __WEB_WEBSERVER_HPP__
