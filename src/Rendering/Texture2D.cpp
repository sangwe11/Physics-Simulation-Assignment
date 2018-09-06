#include "Texture2D.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Texture2D::Texture2D(const std::string &name, const bool &mipmaps, const bool &anisotropic, const unsigned int &samples)
{
	SDL_Surface *surface = IMG_Load(name.c_str());

	if (surface == nullptr)
	{
		std::cout << "Failed to load texture: " << name << std::endl;
		return;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Buffer texture
	switch (surface->format->format)
	{
	case SDL_PIXELFORMAT_RGB24:
	case SDL_PIXELFORMAT_RGB332:
	case SDL_PIXELFORMAT_RGB444:
	case SDL_PIXELFORMAT_RGB555:
	case SDL_PIXELFORMAT_RGB565:
	case SDL_PIXELFORMAT_RGB888:
		// RGB format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		break;
	case SDL_PIXELFORMAT_RGBA4444:
	case SDL_PIXELFORMAT_RGBA5551:
	case SDL_PIXELFORMAT_RGBA8888:
		// RGBA format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		break;
	case SDL_PIXELFORMAT_BGR24:
	case SDL_PIXELFORMAT_BGR555:
	case SDL_PIXELFORMAT_BGR565:
	case SDL_PIXELFORMAT_BGR888:
		// BGR format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
		break;
	case SDL_PIXELFORMAT_ABGR1555:
	case SDL_PIXELFORMAT_ABGR4444:
	case SDL_PIXELFORMAT_ABGR8888:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		break;
	case SDL_PIXELFORMAT_ARGB1555:
	case SDL_PIXELFORMAT_ARGB2101010:
	case SDL_PIXELFORMAT_ARGB4444:
	case SDL_PIXELFORMAT_ARGB8888:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		break;
	default:
		std::cout << "Unknown texture format: " << SDL_GetPixelFormatName(surface->format->format) << std::endl;
		glDeleteTextures(1, &m_texture);
		return;
		break;
	}

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repeat wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repeat wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear mag filtering

	// Generate mipmaps if desired
	if (mipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // trilinear min filtering
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear min filtering
	}

	// Anisotropic filtering if desired
	if (anisotropic)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)samples);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Free SDL surface
	SDL_FreeSurface(surface);
	surface = nullptr;

	std::cout << "Loaded texture: " << name << std::endl;
}

Texture2D::~Texture2D()
{
	// Delete texture
	glDeleteTextures(1, &m_texture);
}

const GLuint Texture2D::get() const
{
	return m_texture;
}

void Texture2D::use(const unsigned int &unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}