#ifndef __GPIO_INPUT_HPP__
#define __GPIO_INPUT_HPP__

#include <QJSValue>
#include <QLoggingCategory>
#include <QObject>
#include <QTimer>
#include <gpiod.hpp>

class Input : public QObject {
    Q_OBJECT

   private:
    int _pin = -1;
    bool _value = false;
    gpiod::chip _chip;
    gpiod::line _line;
    QJSValue _callback;

   public:
    Input(const Input &) = delete;
    Input &operator=(const Input &) = delete;
    Input(const Input &&) = delete;
    Input &operator=(const Input &&) = delete;
    Input(int pin, QJSValue &callback);
    ~Input() override;

    void read();
};

#endif  // __GPIO_INPUT_HPP__
