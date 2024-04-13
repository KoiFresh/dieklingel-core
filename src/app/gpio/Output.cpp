#include "Output.hpp"

Output::Output() {}

Output::~Output() {}

int Output::getPin() { return this->_pin; }

void Output::setPin(int pin) {
    if (pin == this->_pin) {
        return;
    }

    if (this->_pin >= 0) {
        this->_line.release();
    }
    this->_pin = pin;

    try {
        auto chip = gpiod::chip("pinctrl-bcm2711", gpiod::chip::OPEN_LOOKUP);
        gpiod::line_request request;
        request.request_type = gpiod::line_request::DIRECTION_OUTPUT;
        auto line = chip.get_line(pin);
        line.request(request);

        this->_chip = chip;
        this->_line = line;
    } catch (std::exception const &exception) {
        this->_pin = -1;
        qWarning(
        ) << "An error ouccured while requesting the pin"
          << pin
          << " on chip 'pinctrl-bcm2711'. This causes that the 'Input' "
             "will not work as excpected. The error message was:"
          << exception.what();
    }

    pinChanged();
    setValue(this->_value);
}

bool Output::getValue() { return this->_value; }

void Output::setValue(bool value) {
    if (this->_pin <= 0) {
        return;
    }

    this->_line.set_value(value);
    if (value != this->_value) {
        this->_value = value;
        valueChanged();
    }
}
