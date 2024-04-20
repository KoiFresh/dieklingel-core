#include "Input.hpp"

Input::Input(int pin, QJSValue &callback) : _pin(pin), _callback(callback) {
    try {
        auto chip = gpiod::chip("pinctrl-bcm2711", gpiod::chip::OPEN_LOOKUP);
        gpiod::line_request request;
        request.request_type = gpiod::line_request::DIRECTION_INPUT;
        auto line = chip.get_line(pin);
        line.request(request);

        this->_chip = chip;
        this->_line = line;
    } catch (std::exception const &exception) {
        this->_pin = -1;
        qWarning() << "An error ouccured while requesting the pin" << pin
                   << " on chip 'pinctrl-bcm2711'. This causes that the 'Input' "
                      "will not work as excpected. The error message was:"
                   << exception.what();
    }
}

Input::~Input() { this->_line.release(); }

void Input::read() {
    if (this->_pin < 0) {
        return;
    }

    bool value = _line.get_value();
    bool hasChanged = value != _value;

    this->_value = value;
    if (hasChanged) {
        this->_callback.call({value});
    }
}
