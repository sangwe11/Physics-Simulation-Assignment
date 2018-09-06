#ifndef _MOUSE_H
#define _MOUSE_H

#include "InputDevice.h"

class Mouse : public InputDevice
{
public:
	Mouse();

	void update();
	void handle_event(const SDL_Event &e);

	void bind_button(const uint8_t &mouse_button, const Button &button);
	void unbind_button(const uint8_t &mouse_button, const Button &button);

	void sensitivity(const float &sensitivity) { m_sensitivity = sensitivity; }
	const float &sensitivity() { return m_sensitivity; }

	void toggle_capture();

private:
	std::map<uint8_t, Button> m_button_bindings;

	float m_sensitivity = 1.0f;
	bool m_captured = true;
};

#endif