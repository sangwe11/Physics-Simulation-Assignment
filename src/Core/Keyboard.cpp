#include "Keyboard.h"

Keyboard::Keyboard()
{
	m_id = 0;
	m_name = "Keyboard";
	m_keystate = SDL_GetKeyboardState(NULL);

	// Bind default keyboard mapping
	bind_button(SDL_SCANCODE_ESCAPE, "back");
	bind_button(SDL_SCANCODE_SPACE, "jump");
	bind_button(SDL_SCANCODE_LSHIFT, "sprint");
	bind_button(SDL_SCANCODE_RSHIFT, "sprint");
	bind_axis(SDL_SCANCODE_W, SDL_SCANCODE_S, "moveY");
	bind_axis(SDL_SCANCODE_A, SDL_SCANCODE_D, "moveX");
}

void Keyboard::update()
{
	// Copy button state to old button state
	m_buttons_old = m_buttons;

	m_buttons.clear();

	// Update button states
	for (std::pair<SDL_Scancode, Button> binding : m_button_bindings)
	{
		if (m_keystate[binding.first])
		{
			m_buttons[binding.second] = true;
		}
	}

	// Update axes
	for (std::pair<std::pair<SDL_Scancode, SDL_Scancode>, Axis> binding : m_axis_bindings)
	{
		float val = 0.0f;

		if (m_keystate[binding.first.first])
			val -= 1.0f;

		if (m_keystate[binding.first.second])
			val += 1.0f;

		m_axes[binding.second] = val;
	}
}

void Keyboard::handle_event(const SDL_Event &e)
{
	// Nothing to see here..
}

void Keyboard::bind_button(const SDL_Scancode &key, const Button &button)
{
	m_button_bindings[key] = button;
}

void Keyboard::unbind_button(const SDL_Scancode &key, const Button &button)
{
	m_button_bindings.erase(key);
}

void Keyboard::bind_axis(const SDL_Scancode &key_neg, const SDL_Scancode &key_pos, const Axis &axis)
{
	m_axis_bindings[std::make_pair(key_neg, key_pos)] = axis;
}

void Keyboard::unbind_axis(const SDL_Scancode &key_neg, const SDL_Scancode &key_pos, const Axis &axis)
{
	m_axis_bindings.erase(std::make_pair(key_neg, key_pos));
}