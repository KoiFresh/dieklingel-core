#include "WebServer.hpp"

WebServer::WebServer() {}

WebServer::~WebServer() {}

void WebServer::port(int value) { this->_port = value; }

void WebServer::onSetupCompleted() {}

void WebServer::print(QTextStream &log) {
    log << "🌍 Web enabled:" << Qt::endl;
    log << "\t NOTE: This is only a placeholder for a planned feature, which "
           "is not implemented yet."
        << Qt::endl;
}
