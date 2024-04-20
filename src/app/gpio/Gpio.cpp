#include "Gpio.hpp"

Gpio::Gpio(std::shared_ptr<QJSEngine> engine) : _engine(std::move(engine)) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::ObjectOwnership::CppOwnership);

    const int GPIO_PIN_READ_INTERVALL_MS = 50;
    this->_timer.setInterval(GPIO_PIN_READ_INTERVALL_MS);
    connect(&this->_timer, &QTimer::timeout, this, &Gpio::_read);
}

Gpio::~Gpio() = default;

void Gpio::input(int pin, QJSValue callback) {
    if (this->_outputs.contains(pin)) {
        throw std::invalid_argument(QString("A pin should only be used once. The pin %1 is already "
                                            "declared as output.")
                                        .arg(pin)
                                        .toStdString());
    }

    if (this->_inputs.contains(pin)) {
        throw std::invalid_argument(QString("A pin should only be used once. The pin %1 is already "
                                            "declared as input.")
                                        .arg(pin)
                                        .toStdString());
    }

    std::shared_ptr<Input> input = std::make_shared<Input>(pin, callback);
    QQmlEngine::setObjectOwnership(input.get(), QQmlEngine::ObjectOwnership::CppOwnership);

    this->_inputs.insert(pin, input);
}

QJSValue Gpio::output(int pin) {
    if (this->_inputs.contains(pin)) {
        throw std::invalid_argument(QString("A pin should only be used once. The pin %1 is already "
                                            "declared as input.")
                                        .arg(pin)
                                        .toStdString());
    }

    if (!this->_outputs.contains(pin)) {
        if (this->_isSetupCompleted) {
            throw std::invalid_argument(QString("An output should be configured during startup before "
                                                "used. The pin %1 was not configured.")
                                            .arg(pin)
                                            .toStdString());
        }

        std::shared_ptr<Output> output = std::make_shared<Output>(pin);
        QQmlEngine::setObjectOwnership(output.get(), QQmlEngine::ObjectOwnership::CppOwnership);

        this->_outputs.insert(pin, output);
    }

    return this->_engine->newQObject(this->_outputs.value(pin).get());
}

void Gpio::onSetupCompleted() {
    this->_isSetupCompleted = true;
    if (this->_inputs.size() > 0) {
        this->_timer.start();
    }
}

void Gpio::print(QTextStream &log) {
    log << "🕹️  Gpio enabled:" << Qt::endl;

    if (this->_inputs.size() > 0) {
        log << "\tinputs:" << Qt::endl;
        for (auto input : this->_inputs.keys()) {
            log << "\t\t- " << input << Qt::endl;
        }
    }

    if (this->_outputs.size() > 0) {
        log << "\toutputs:" << Qt::endl;
        for (auto output : this->_outputs.keys()) {
            log << "\t\t- " << output << Qt::endl;
        }
    }
}

void Gpio::_read() {
    for (std::shared_ptr<Input> input : this->_inputs.values()) {
        input->read();
    }
}
