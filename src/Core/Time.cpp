#include "Time.h"

#include <SDL2/SDL.h>

void Time::start()
{
	m_last = SDL_GetTicks();
}

void Time::end()
{
	unsigned int current = SDL_GetTicks();
	unsigned int frame = current - m_last;

	m_delta = frame / 1000.0f;
}

void Time::update()
{
	// Update delta time
	unsigned int current = SDL_GetTicks();
	unsigned int frame = current - m_last;
	m_last = current;
	m_delta = frame / 1000.0f;
}