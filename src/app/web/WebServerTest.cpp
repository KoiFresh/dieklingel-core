#include <gmock/gmock-matchers.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>

#include "WebServer.hpp"

TEST(WebServer, printStatesWebEnabled) {
    auto server = WebServer();
    QString log;
    QTextStream stream(&log);

    server.print(stream);

    EXPECT_THAT(log.toStdString(), testing::HasSubstr("Web enabled"));
}
