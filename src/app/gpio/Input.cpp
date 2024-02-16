#include "Input.hpp"

Input::Input()
{
	this->_timer.setInterval(50);
	connect(&this->_timer, &QTimer::timeout, this, &Input::_update);
}

Input::~Input()
{
}

void Input::_update()
{
	bool value = _line.get_value();
	bool isRising = value && !_value;
	bool isFalling = !value && _value;
	bool hasChanged = value != _value;

	this->_value = value;
	if (isRising)
	{
		risingEdge();
	}
	if (isFalling)
	{
		fallingEdge();
	}
	if (hasChanged)
	{
		valueChanged();
	}
}

int Input::getPin()
{
	return this->_pin;
}

void Input::setPin(int pin)
{
	if (pin == this->_pin)
	{
		return;
	}

	this->_timer.stop();
	if (this->_pin >= 0)
	{
		this->_line.release();
	}
	this->_pin = pin;

	try
	{
		auto chip = gpiod::chip("pinctrl-bcm2711", gpiod::chip::OPEN_LOOKUP);
		gpiod::line_request request;
		request.request_type = gpiod::line_request::DIRECTION_INPUT;
		auto line = chip.get_line(pin);
		line.request(request);

		this->_chip = chip;
		this->_line = line;
		this->_timer.start();
	}
	catch (std::exception const &exception)
	{
		this->_pin = -1;
		qWarning() << "An error ouccured while requesting the pin" << pin << " on chip 'pinctrl-bcm2711'. This causes that the 'Input' will not work as excpected. The error message was:" << exception.what();
	}

	pinChanged();
}

bool Input::getValue()
{
	return this->_value;
}
