#include "Setup.hpp"

Setup::Setup(int& argc, char** argv) : _argc(argc), _argv(argv) {
    this->_application = new QCoreApplication(argc, argv);
    this->_engine = new QQmlApplicationEngine();
    this->_engine->installExtensions(QJSEngine::AllExtensions);
}

Setup::~Setup() {
    delete this->_engine;
    this->_engine = nullptr;
}

void Setup::useGui() {
    bool isGui =
        (qobject_cast<QGuiApplication*>(QCoreApplication::instance()) != 0);

    if (isGui) {
        return;
    }

    delete this->_application;
    this->_application = nullptr;

    // Use a QGuiApplication instead of the existing one, but only if
    // Kiosk::entry is configured
    this->_application = new QGuiApplication(this->_argc, this->_argv);
}

Setup* Setup::script(QString file) {
    QQmlEngine::setObjectOwnership(
        this,
        QQmlEngine::ObjectOwnership::CppOwnership
    );

    QJSValue conf = this->_engine->newQObject(this);
    this->_engine->globalObject().setProperty(
        "configure",
        conf.property("configure")
    );
    this->_engine->globalObject().setProperty("use", conf.property("use"));

    this->_file = file;
    return this;
}

Setup* Setup::directory(QString directory) {
    this->_directories.append(directory);
    return this;
}

Setup* Setup::configureable(QString name, ConfigurationFactory factory) {
    if (this->_factories.contains(name)) {
        throw "Cannot register two configureables with the name " + name;
    }

    this->_factories.insert(name, factory);
    return this;
}

int Setup::exec() {
    if (this->_file.isEmpty()) {
        qWarning() << "Cannot exec the setup, when the JS file is not set.";
        return -1;
    }
    if (this->_directories.isEmpty()) {
        this->_directories.append(".");
    }

    for (QString path : this->_directories) {
        QString uri = path + "/" + this->_file;
        if (QFile::exists(uri)) {
            return _exec(uri);
        }
    }

    return -3;
}

int Setup::_exec(QString uri) {
    qInfo() << qPrintable(QString("🏗️  Setup dieklingel-core from %1.").arg(uri)
    );

    QFile script(uri);
    if (!script.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot read the setup file from " << uri;
        return -2;
    }

    QString contents = script.readAll();
    QJSValue result = this->_engine->evaluate(contents, this->_file);

    if (result.isError()) {
        qWarning() << result.toString();
        return -4;
    }

    _isSetupCompleted = true;
    emit whenSetupCompletes();

    QTextStream log(stdout);
    log.setCodec("ISO 8859-1");
    for (auto integration : this->_integrations.values()) {
        integration->print(log);
    }

    if (this->_integrations.size() == 0) {
        qInfo() << "🚀 Woah! You are running dieklingel-core without a single "
                   "integration enabled, so nothing will happen. Check out "
                   "https://dieklingel.com/ for more information.";
    }

    return this->_application->exec();
}

Configuration* Setup::require(QString section) {
    if (this->_isSetupCompleted) {
        if (!this->_integrations.contains(section)) {
            throw std::logic_error(
                QString("An integration with the identifier %1 was not "
                        "requested, before the setup completed, this it will "
                        "not be constructed.")
                    .arg(section)
                    .toStdString()
            );
        }
        auto integration = this->_integrations.value(section);
        return integration;
    }

    if (this->_factories.contains(section)) {
        auto factory = this->_factories.take(section);
        auto instance = factory();
        QQmlEngine::setObjectOwnership(
            instance,
            QQmlEngine::ObjectOwnership::CppOwnership
        );

        connect(
            this,
            &Setup::whenSetupCompletes,
            instance,
            &Configuration::onSetupCompleted
        );

        this->_integrations.insert(section, instance);
    }

    if (!this->_integrations.contains(section)) {
        throw std::logic_error(QString("A integration with %1 does not exists.")
                                   .arg(section)
                                   .toStdString());
    }
    auto integration = this->_integrations.value(section);
    return integration;
}

void Setup::configure(QJSValue section, QJSValue callback) {
    if (!section.toBool()) {
        return;
    }

    auto identifier = section.toString();
    if (this->_isSetupCompleted) {
        this->_engine->throwError(QString("Cannot configgure(%1) after the "
                                          "setup is completed. Use use(%1)")
                                      .arg(identifier));
    }

    if (this->_factories.contains(identifier)) {
        auto factory = this->_factories.take(identifier);
        auto instance = factory();
        QQmlEngine::setObjectOwnership(
            instance,
            QQmlEngine::ObjectOwnership::CppOwnership
        );

        connect(
            this,
            &Setup::whenSetupCompletes,
            instance,
            &Configuration::onSetupCompleted
        );

        this->_integrations.insert(identifier, instance);
    }

    if (!this->_integrations.contains(identifier)) {
        this->_engine->throwError(
            QString("A configuration with %1 does not exists.").arg(identifier)
        );
        return;
    }

    auto prototype = this->_integrations.value(identifier);
    QJSValue obj = this->_engine->newQObject(prototype);
    QJSValueList args = {obj};
    callback.call(args);
}

QJSValue Setup::use(QString section) {
    if (this->_factories.contains(section)) {
        if (this->_isSetupCompleted) {
            this->_engine->throwError(
                QString("Cannot use(%1) before creation.").arg(section)
            );
            return QJSValue();
        }

        auto factory = this->_factories.take(section);
        auto instance = factory();
        QQmlEngine::setObjectOwnership(
            instance,
            QQmlEngine::ObjectOwnership::CppOwnership
        );

        connect(
            this,
            &Setup::whenSetupCompletes,
            instance,
            &Configuration::onSetupCompleted
        );

        this->_integrations.insert(section, instance);
    }

    if (!this->_integrations.contains(section)) {
        this->_engine->throwError(
            QString("A configuration with %1 does not exists.").arg(section)
        );
        return QJSValue();
    }

    auto prototype = this->_integrations.value(section);
    QJSValue obj = this->_engine->newQObject(prototype);
    return obj;
}

QQmlApplicationEngine* Setup::engine() { return this->_engine; }
