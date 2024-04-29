#ifndef __GPIO_GPIO_HPP__
#define __GPIO_GPIO_HPP__

#include <QQmlEngine>
#include <QTextStream>

#include "../setup/Configuration.hpp"
#include "Input.hpp"
#include "Output.hpp"

class Gpio : public Configuration {
    Q_OBJECT
   private:
    std::shared_ptr<QJSEngine> _engine;
    QTimer _timer;
    bool _isSetupCompleted = false;
    QMap<int, std::shared_ptr<Input>> _inputs;
    QMap<int, std::shared_ptr<Output>> _outputs;

    void _read();

   public:
    Gpio(const Gpio &) = delete;
    Gpio &operator=(const Gpio &) = delete;
    Gpio(const Gpio &&) = delete;
    Gpio &operator=(const Gpio &&) = delete;
    Gpio(std::shared_ptr<QJSEngine> engine);
    ~Gpio() override;

    Q_INVOKABLE void input(int pin, QJSValue callback);
    Q_INVOKABLE QJSValue output(int pin);

    void onSetupCompleted() override;
    void print(QTextStream &log) override;
};

#endif  // __GPIO_GPIO_HPP__
