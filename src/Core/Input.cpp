#include "Input.h"

Input::Input()
{
}

void Input::update()
{
	// Clear events
	m_events.clear();

	// Update input devices
	m_keyboard.update();
	m_mouse.update();

	// Poll SDL events
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_QUIT:
			m_events["quit"] = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
			m_mouse.handle_event(e);
			break;
		default:
			// unhandled event
			break;
		}

	}
}