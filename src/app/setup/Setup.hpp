#ifndef __SETUP_SETUP_HPP__
#define __SETUP_SETUP_HPP__

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QException>
#include <QFile>
#include <QGuiApplication>
#include <QJSEngine>
#include <QQmlApplicationEngine>
#include <QString>
#include <QStringList>

#include "Configuration.hpp"

namespace Core {

using ConfigurationFactory = std::function<std::shared_ptr<Configuration>()>;

class Setup : public QObject, public std::enable_shared_from_this<Setup> {
    Q_OBJECT
   private:
    std::shared_ptr<QCoreApplication> _application;
    QString _file;
    QStringList _directories;
    std::shared_ptr<QQmlApplicationEngine> _engine;
    QMap<QString, ConfigurationFactory> _factories;
    QMap<QString, std::shared_ptr<Configuration>> _integrations;
    bool _isSetupCompleted = false;

    int _exec(QString uri);
    int& _argc;
    char** _argv;

   public:
    static QString getVersion();

    Setup(const Setup&) = delete;
    Setup& operator=(const Setup&) = delete;
    Setup(const Setup&&) = delete;
    Setup& operator=(const Setup&&) = delete;
    Setup(int& argc, char** argv);
    ~Setup() override;

    void useGui();

    std::shared_ptr<Setup> script(QString file);
    std::shared_ptr<Setup> directory(QString directory);

    std::shared_ptr<Setup> configureable(QString name, ConfigurationFactory factory);

    int exec();
    std::shared_ptr<QQmlApplicationEngine> engine();

    std::shared_ptr<Configuration> require(QString section);

    template <typename T>
    std::shared_ptr<T> require(QString section) {
        auto shared = require(section);

        T* integration = qobject_cast<T*>(shared.get());
        if (integration == nullptr) {
            throw std::logic_error(
                QString("The integration %1 is not of the requested type.").arg(section).toStdString()
            );
        }

        return std::dynamic_pointer_cast<T>(shared);
    }

    Q_INVOKABLE void configure(QJSValue section, QJSValue callback);
    Q_INVOKABLE QJSValue use(QString section);

   signals:
    void whenSetupCompletes();
};

}  // namespace Core

#endif  // __SETUP_SETUP_HPP__