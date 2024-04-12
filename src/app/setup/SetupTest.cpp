
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Setup.hpp"

TEST(Setup, creates_QCoreApplication) {
    EXPECT_TRUE(QCoreApplication::instance() == nullptr);

    int argc = 0;
    char **argv = nullptr;
    auto setup = Core::Setup(argc, argv);

    EXPECT_TRUE(QCoreApplication::instance() != nullptr);
}

TEST(Setup, destruction_deletes_QCoreApplication) {
    int argc = 0;
    char **argv = nullptr;
    auto setup = new Core::Setup(argc, argv);
    delete setup;

    EXPECT_TRUE(QCoreApplication::instance() == nullptr);
}

TEST(Setup, useGui_replaces_QCoreApplication_with_QGuiApplication) {
    int argc = 0;
    char **argv = nullptr;
    auto setup = Core::Setup(argc, argv);

    qputenv("QT_QPA_PLATFORM", "vnc");
    setup.useGui();

    auto gui = qobject_cast<QGuiApplication *>(QCoreApplication::instance());
    EXPECT_TRUE(gui != nullptr);
}
