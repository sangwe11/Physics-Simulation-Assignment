#ifndef _TEXTURE2D_H
#define _TEXTURE2D_h

#include <string>
#include <GL/glew.h>

class Texture2D
{
public:
	Texture2D(const std::string &name, const bool &mipmaps, const bool &anisotropic, const unsigned int &samples);
    ~Texture2D();

	const GLuint get() const;
	void use(const unsigned int &unit);

private:
	GLuint m_texture;
};

#endif