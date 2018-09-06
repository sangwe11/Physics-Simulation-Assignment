#include "Mouse.h"

Mouse::Mouse()
{
	m_id = 0;
	m_name = "Mouse";

	// Bind default mouse mapping
	bind_button(SDL_BUTTON_LEFT, "fire");
	bind_button(SDL_BUTTON_RIGHT, "aim");

	SDL_SetRelativeMouseMode(SDL_TRUE);
}


void Mouse::update()
{
	m_buttons_old = m_buttons;
	m_axes.clear();
}

void Mouse::handle_event(const SDL_Event &e)
{
	switch (e.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		m_buttons[m_button_bindings[e.button.button]] = true;
		break;

	case SDL_MOUSEBUTTONUP:
		m_buttons[m_button_bindings[e.button.button]] = false;
		break;

	case SDL_MOUSEMOTION:		
		m_axes["mouseX"] = -e.motion.xrel * m_sensitivity;
		m_axes["mouseY"] = -e.motion.yrel * m_sensitivity;
		break;
	}
}


void Mouse::bind_button(const uint8_t &mouse_button, const Button &button)
{
	m_button_bindings[mouse_button] = button;
}

void Mouse::unbind_button(const uint8_t &mouse_button, const Button &button)
{
	m_button_bindings.erase(mouse_button);
}

void Mouse::toggle_capture()
{
	if (m_captured)
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		m_captured = false;
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		m_captured = true;
	}
}