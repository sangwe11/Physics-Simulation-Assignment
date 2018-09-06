#ifndef _INPUTDEVICE_H
#define _INPUTDEVICE_H

#include <string>
#include <map>
#include <vector>

#include <SDL2/SDL.h>

class InputDevice
{
public:
	typedef std::string Button;
	typedef std::string Axis;

	virtual void update() = 0;
	virtual void handle_event(const SDL_Event &e) = 0;

	const int &id() { return m_id; }
	const std::string &name() { return m_name; }

	const bool button(const Button &button);
	const bool button_down(const Button &button);
	const bool button_up(const Button &button);
	const float axis(const Axis &axis);

protected:
	int m_id;
	std::string m_name;

	std::map<Button, bool> m_buttons;
	std::map<Button, bool> m_buttons_old;
	std::map<Axis, float> m_axes;
};

#endif