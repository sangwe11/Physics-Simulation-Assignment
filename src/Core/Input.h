#ifndef _INPUT_H
#define _INPUT_H

#include "Keyboard.h"
#include "Mouse.h"

class Input
{
public:
	typedef std::string Event;

	Input();

	Input(Input const &) = delete;
	void operator=(Input const &) = delete;

	void update();

	const bool event(const Event &e) { return m_events[e]; }

	Keyboard &keyboard() { return m_keyboard; }
	Mouse &mouse() { return m_mouse; }

private:
	Keyboard m_keyboard;
	Mouse m_mouse;

	std::map<Event, bool> m_events;
};

#endif