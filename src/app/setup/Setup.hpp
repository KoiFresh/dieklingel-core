#ifndef __SETUP_SETUP_HPP__
#define __SETUP_SETUP_HPP__

#include <QCoreApplication>
#include <QDebug>
#include <QException>
#include <QFile>
#include <QGuiApplication>
#include <QJSEngine>
#include <QQmlApplicationEngine>
#include <QString>
#include <QStringList>

#include "Configuration.hpp"

typedef std::function<Configuration*()> ConfigurationFactory;

class Setup : public QObject {
    Q_OBJECT
   private:
    QCoreApplication* _application = nullptr;
    QString _file;
    QStringList _directories;
    QQmlApplicationEngine* _engine = nullptr;
    QMap<QString, ConfigurationFactory> _factories;
    QMap<QString, Configuration*> _integrations;
    bool _isSetupCompleted = false;

    int _exec(QString uri);
    int& _argc;
    char** _argv;

   public:
    Setup(int& argc, char** argv);
    ~Setup();

    void useGui();

    Setup* script(QString file);
    Setup* directory(QString directory);

    Setup* configureable(QString name, ConfigurationFactory factory);

    int exec();
    QQmlApplicationEngine* engine();

    Configuration* require(QString section);

    template <typename T>
    T* require(QString section) {
        auto integration = qobject_cast<T*>(require(section));
        if (integration == nullptr) {
            throw std::logic_error(
                QString("The integration %1 is not of the requested type.")
                    .arg(section)
                    .toStdString()
            );
        }
        return integration;
    }

    Q_INVOKABLE void configure(QJSValue section, QJSValue callback);
    Q_INVOKABLE QJSValue use(QString section);

   signals:
    void whenSetupCompletes();
};

#endif  // __SETUP_SETUP_HPP__