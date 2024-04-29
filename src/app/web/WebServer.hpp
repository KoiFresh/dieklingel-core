#ifndef __WEB_WEBSERVER_HPP__
#define __WEB_WEBSERVER_HPP__

#include <QDebug>
#include <QObject>

#include "../Constants.hpp"
#include "../setup/Configuration.hpp"

class WebServer : public Configuration {
    Q_OBJECT
   private:
    int _port = HTTP_DEFAULT_PORT;

   public:
    WebServer(const WebServer &) = delete;
    WebServer &operator=(const WebServer &) = delete;
    WebServer(const WebServer &&) = delete;
    WebServer &operator=(const WebServer &&) = delete;
    WebServer();
    ~WebServer() override;

    void onSetupCompleted() override;
    void print(QTextStream &log) override;

    Q_INVOKABLE void port(int value);
};

#endif  // __WEB_WEBSERVER_HPP__
