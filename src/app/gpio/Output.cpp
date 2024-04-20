#include "Output.hpp"

Output::Output(int pin) {
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
        qWarning() << "An error ouccured while requesting the pin" << pin
                   << " on chip 'pinctrl-bcm2711'. This causes that the 'Output' "
                      "will not work as excpected. The error message was:"
                   << exception.what();
    }
}

Output::~Output() = default;

void Output::setValue(bool value) {
    if (this->_pin < 0) {
        return;
    }

    this->_line.set_value(value);
}

void Output::high() {
    if (this->_pin < 0) {
        return;
    }

    this->setValue(true);
}

void Output::low() {
    if (this->_pin < 0) {
        return;
    }

    this->setValue(false);
}
