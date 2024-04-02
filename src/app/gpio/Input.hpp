#ifndef __GPIO_INPUT_HPP__
#define __GPIO_INPUT_HPP__

#include <QLoggingCategory>
#include <QObject>
#include <QTimer>
#include <gpiod.hpp>

class Input : public QObject {
  Q_OBJECT
  Q_PROPERTY(int pin READ getPin WRITE setPin NOTIFY pinChanged)
  Q_PROPERTY(bool value READ getValue NOTIFY valueChanged)
 private:
  int _pin = -1;
  bool _value = false;
  gpiod::chip _chip;
  gpiod::line _line;
  QTimer _timer;

  void _update();

 public:
  Input();
  ~Input();

  int getPin();
  void setPin(int pin);
  bool getValue();

 signals:
  void pinChanged();
  void valueChanged();
  void risingEdge();
  void fallingEdge();
};

#endif  // __GPIO_INPUT_HPP__
