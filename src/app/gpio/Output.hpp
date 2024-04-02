#ifndef __GPIO_OUTPUT_HPP__
#define __GPIO_OUTPUT_HPP__

#include <QLoggingCategory>
#include <QObject>
#include <QTimer>
#include <gpiod.hpp>

class Output : public QObject {
  Q_OBJECT
  Q_PROPERTY(int pin READ getPin WRITE setPin NOTIFY pinChanged)
  Q_PROPERTY(bool value READ getValue WRITE setValue NOTIFY valueChanged)
 private:
  int _pin = -1;
  bool _value = false;
  gpiod::chip _chip;
  gpiod::line _line;

 public:
  Output();
  ~Output();

  int getPin();
  void setPin(int pin);
  bool getValue();
  void setValue(bool value);

 signals:
  void pinChanged();
  void valueChanged();
};

#endif  // __GPIO_OUTPUT_HPP__
