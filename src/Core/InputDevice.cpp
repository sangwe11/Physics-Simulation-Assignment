#include "InputDevice.h"

const bool InputDevice::button(const Button &button)
{
	return m_buttons[button];
}

const bool InputDevice::button_down(const Button &button)
{
	return (m_buttons[button] && !m_buttons_old[button]);
}

const bool InputDevice::button_up(const Button &button)
{
	return (!m_buttons[button] && m_buttons_old[button]);
}

const float InputDevice::axis(const Axis &axis)
{
	return m_axes[axis];
}