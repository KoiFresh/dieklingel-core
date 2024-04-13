
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Setup.hpp"

static int argc = 0;
static char **argv = nullptr;

TEST(Setup, creates_QCoreApplication) {
    EXPECT_TRUE(QCoreApplication::instance() == nullptr);

    auto setup = Core::Setup(argc, argv);

    EXPECT_TRUE(QCoreApplication::instance() != nullptr);
}

TEST(Setup, creates_QQmlApplicationEngine) {
    auto setup = Core::Setup(argc, argv);

    EXPECT_TRUE(setup.engine() != nullptr);
}

TEST(Setup, destruction_deletes_QCoreApplication) {
    auto setup = new Core::Setup(argc, argv);
    delete setup;

    EXPECT_TRUE(QCoreApplication::instance() == nullptr);
}

TEST(Setup, useGui_replaces_QCoreApplication_with_QGuiApplication) {
    auto setup = Core::Setup(argc, argv);

    qputenv("QT_QPA_PLATFORM", "vnc");
    setup.useGui();

    auto gui = qobject_cast<QGuiApplication *>(QCoreApplication::instance());
    EXPECT_TRUE(gui != nullptr);
}

TEST(Setup, exec_cannot_be_run_without_script) {
    auto setup = Core::Setup(argc, argv);

    int result = setup.exec();

    EXPECT_EQ(result, -1);
}
