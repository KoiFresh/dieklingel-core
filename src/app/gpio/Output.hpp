#ifndef __GPIO_OUTPUT_HPP__
#define __GPIO_OUTPUT_HPP__

#include <QLoggingCategory>
#include <QObject>
#include <QTimer>
#include <gpiod.hpp>

class Output : public QObject {
    Q_OBJECT
   private:
    int _pin = -1;
    gpiod::chip _chip;
    gpiod::line _line;

   public:
    Output(const Output &) = delete;
    Output &operator=(const Output &) = delete;
    Output(const Output &&) = delete;
    Output &operator=(const Output &&) = delete;
    Output(int pin);
    ~Output() override;

    void setValue(bool value);

    Q_INVOKABLE void high();
    Q_INVOKABLE void low();
};

#endif  // __GPIO_OUTPUT_HPP__
