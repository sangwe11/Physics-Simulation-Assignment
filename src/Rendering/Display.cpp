#include "Display.h"

Display::Display(const unsigned int &width, const unsigned int &height, const std::string &title)
{
	m_width = width;
	m_height = height;

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	m_context = SDL_GL_CreateContext(m_window);

	// Glew
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		SDL_Quit();

		throw std::exception("Glew init failed");
	}

	// OpenGL settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	SDL_GL_SetSwapInterval(1);
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
}

void Display::swap()
{
	SDL_GL_SwapWindow(m_window);
}

void Display::clear()
{
    glClearDepth(1.0f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}