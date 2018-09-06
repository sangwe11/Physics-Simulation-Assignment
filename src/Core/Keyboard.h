#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "InputDevice.h"

class Keyboard : public InputDevice
{
public:
	Keyboard();

	void update();
	void handle_event(const SDL_Event &e);

	void bind_button(const SDL_Scancode &key, const Button &button);
	void unbind_button(const SDL_Scancode &key, const Button &button);

	void bind_axis(const SDL_Scancode &key_neg, const SDL_Scancode &key_pos, const Axis &axis);
	void unbind_axis(const SDL_Scancode &key_neg, const SDL_Scancode &key_pos, const Axis &axis);

private:
	const Uint8 *m_keystate;

	std::map<SDL_Scancode, Button> m_button_bindings;
	std::map<std::pair<SDL_Scancode, SDL_Scancode>, Axis> m_axis_bindings;
};

#endif