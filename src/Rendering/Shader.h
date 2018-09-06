#ifndef _SHADER_H
#define _SHADER_H

#include <string>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>

static const std::string readFile(std::string fileName);
static GLuint createShader(const std::string &name, const GLenum &type);
static bool checkShaderError(GLuint shader, GLuint flag, const std::string & errorMessage);
static bool checkProgramError(GLuint program, GLuint flag, const std::string & errorMessage);

class Shader
{
public:
	Shader(const std::string &name);
	~Shader();

	const GLuint id() const;
	void use() const;

	const std::string &name() const { return m_name; }

	// Get attrib by name
	const bool getAttrib(const std::string &name, GLuint &attribOut) const;

	// Get uniform by name
	const bool getUniform(const std::string &name, GLuint &uniformOut) const;

	// Set uniform by name
	void setUniform(const std::string &name, const glm::mat4 &value);
	void setUniform(const std::string &name, const glm::mat3 &value);
	void setUniform(const std::string &name, const glm::vec4 &value);
	void setUniform(const std::string &name, const glm::vec3 &value);
	void setUniform(const std::string &name, const glm::vec2 &value);
	void setUniform(const std::string &name, const float &value);
	void setUniform(const std::string &name, const int &value);
	void setUniform(const std::string &name, const unsigned int &value);
	void setUniform(const std::string &name, const bool &value);

private:
	std::string m_name;
	
	GLuint m_program;
	GLuint m_vertex;
	GLuint m_frag;

	std::map<std::string, GLuint> m_attribs;
	std::map<std::string, GLuint> m_uniforms;
};

#endif