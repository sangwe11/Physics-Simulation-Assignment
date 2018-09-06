#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>

class Display
{
public:
	Display(const unsigned int &width, const unsigned int &height, const std::string &title);
	~Display();

	//Display(Display const &) = delete; ?????????
	//void operator=(Display const &) = delete; ?????????

	const unsigned int &width() { return m_width; }
	const unsigned int &height() { return m_height; }
        
    void swap();
    void clear();

	const float aspect_ratio() { return (float)m_width / (float)m_height; }

private:
	SDL_Window *m_window = nullptr;
	SDL_GLContext m_context;

	std::string m_title;
	unsigned int m_width;
	unsigned int m_height;
};

#endif