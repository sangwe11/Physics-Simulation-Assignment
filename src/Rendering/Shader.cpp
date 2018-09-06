#include "Shader.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

static const std::string readFile(std::string fileName)
{
	std::string line;
	std::string doc;

	// Open file for reading
	std::ifstream file;
	file.open(fileName.c_str());

	if (file.is_open())
	{
		while (getline(file, line))
		{
			doc.append(line + "\n");
		}

		file.close();
	}
	else
	{
		std::cerr << "Could not open file: " << fileName << std::endl;
	}

	return doc;
}

static GLuint createShader(const std::string &name, const GLenum &type)
{
	std::string ext;

	switch (type)
	{
	case GL_VERTEX_SHADER:
		ext = ".vert";
		break;
	case GL_FRAGMENT_SHADER:
		ext = ".frag";
		break;
	case GL_GEOMETRY_SHADER:
		ext = ".geom";
		break;
	default:
		ext = "";
		break;
	}

	// Create a new shader of shaderType
	GLuint shader = glCreateShader(type);

	std::string shaderSource = readFile(name + ext);

	//std::cout << shaderSource << std::endl;

	// If shader source is empty, then it couldn't be loaded.
	if (shaderSource == "")
		return NULL;

	// Convert source to GLchar *
	const GLchar * source = shaderSource.c_str();
	GLint sourceLength = static_cast<GLint>(shaderSource.length());

	// Pass source to OpenGL and compile shader
	glShaderSource(shader, 1, &source, &sourceLength);
	glCompileShader(shader);

	// Check shader compile status
	if (!checkShaderError(shader, GL_COMPILE_STATUS, "Shader compile failed!"))
		std::cout << "Shader compiled successfully: " << name << ext << std::endl;

	return shader;
}

static bool checkShaderError(GLuint shader, GLuint flag, const std::string & errorMessage)
{
	GLint status;
	GLchar error[1024] = { 0 };

	glGetShaderiv(shader, flag, &status);

	if (status == GL_FALSE)
	{
		glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		std::cout << errorMessage + ": '" << error << "'" << std::endl;
		return true;
	}

	return false;

}

static bool checkProgramError(GLuint program, GLuint flag, const std::string & errorMessage)
{
	GLint status;
	GLchar error[1024] = { 0 };

	glGetProgramiv(program, flag, &status);

	if (status == GL_FALSE)
	{
		glGetProgramInfoLog(program, sizeof(error), NULL, error);
		std::cout << errorMessage + ": '" << error << "'" << std::endl;
		return true;
	}

	return false;

}

Shader::Shader(const std::string &name)
{
	m_name = name;
	m_program = glCreateProgram();

	m_vertex = createShader(name, GL_VERTEX_SHADER);
	m_frag = createShader(name, GL_FRAGMENT_SHADER);

	// Vertex shader is required!
	if (m_vertex != NULL)
		glAttachShader(m_program, m_vertex);
	else
	{
		throw std::exception("Vertex shader is required.");
	}

	// Fragment shader is required!
	if (m_frag != NULL)
		glAttachShader(m_program, m_frag);
	else
	{
		throw std::exception("Fragment shader is required.");
	}

	// Link the shader program
	glLinkProgram(m_program);

	// Check link status
	if (!checkProgramError(m_program, GL_LINK_STATUS, "Shader not linked correctly!"))
		std::cout << "Shader linked correctly!" <<std::endl;

	// Validate the program
	glValidateProgram(m_program);

	// Check program is valid
	if (!checkProgramError(m_program, GL_VALIDATE_STATUS, "Shader not valid!"))
	{
		std::cout << "Shader valid!" << std::endl;
	}

	// Bind the frag shader output variable
	glUseProgram(m_program);
	glBindFragDataLocation(m_program, 0, "finalColor");

	// Store all attribs and uniforms for easier lookup
	GLint numActiveAttribs = 0;
	GLint numActiveUniforms = 0;
	GLint numUniformBuffers = 0;

	glGetProgramInterfaceiv(m_program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);

	// Get and store active attribs
	for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
	{
		// Grab the resource name and store
		std::vector <GLchar> nameData(256);
		glGetProgramResourceName(m_program, GL_PROGRAM_INPUT, attrib, (GLsizei)nameData.size(), NULL, &nameData[0]);

		// Convert to std::string and strip null chars using c++11 lambda
		std::string name((char*)&nameData[0], nameData.size() - 1);
		name.erase(std::remove_if(name.begin(), name.end(), [](char c){ return (c == '\0'); }), name.end());

		// Get location and store
		GLuint location = glGetAttribLocation(m_program, &nameData[0]);
		m_attribs[name] = location;
	}

	glGetProgramInterfaceiv(m_program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

	// Get and store active uniforms
	for (int uniform = 0; uniform < numActiveUniforms; ++uniform)
	{
		// Grab the resource name and store
		std::vector <GLchar> nameData(256);
		glGetProgramResourceName(m_program, GL_UNIFORM, uniform, (GLsizei)nameData.size(), NULL, &nameData[0]);

		// Convert to std::string and strip null chars using c++11 lambda
		std::string name((char*)&nameData[0], nameData.size() - 1);
		name.erase(std::remove_if(name.begin(), name.end(), [](char c){ return (c == '\0'); }), name.end());

		// Get location
		GLuint location = glGetUniformLocation(m_program, &nameData[0]);

		// Check the uniform is not part of a uniform block and store
		if (location != 4294967295)
			m_uniforms[name] = location;
	}
}

Shader::~Shader()
{
	glDetachShader(m_program, m_vertex);
	glDeleteShader(m_vertex);
	glDetachShader(m_program, m_vertex);
	glDeleteShader(m_frag);
	glDeleteProgram(m_program);
}

const GLuint Shader::id() const
{
	return m_program;
}

void Shader::use() const
{
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	if (id != m_program)
		glUseProgram(m_program);
}

const bool Shader::getAttrib(const std::string& name, GLuint& attribOut) const
{
	if (m_attribs.find(name) != m_attribs.end())
	{
		attribOut = m_attribs.at(name);
		return true;
	}

	return false;
}

const bool Shader::getUniform(const std::string& name, GLuint& uniformOut) const
{
	if (m_uniforms.find(name) != m_uniforms.end())
	{
		uniformOut = m_uniforms.at(name);
		return true;
	}

	return false;
}

void Shader::setUniform(const std::string &name, const glm::mat4 &value)
{
	GLuint uniform;
	if (getUniform(name, uniform)) glUniformMatrix4fv(uniform, 1, false, &value[0][0]);
}

void Shader::setUniform(const std::string &name, const glm::mat3 &value)
{
	GLuint uniform;
	if (getUniform(name, uniform)) glUniformMatrix3fv(uniform, 1, false, &value[0][0]);
}

void Shader::setUniform(const std::string &name, const glm::vec4 &value)
{
	GLuint uniform;
	if (getUniform(name, uniform)) glUniform4fv(uniform, 1, &value[0]);
}

void Shader::setUniform(const std::string &name, const glm::vec3 &value)
{
	GLuint uniform;
	if (getUniform(name, uniform)) glUniform3fv(uniform, 1, &value[0]);
}

void Shader::setUniform(const std::string &name, const glm::vec2 &value)
{
	GLuint uniform;
	if (getUniform(name, uniform)) glUniform2fv(uniform, 1, &value[0]);
}

void Shader::setUniform(const std::string &name, const float &value)
{
	GLuint uniform;
	if (getUniform(name, uniform)) glUniform1fv(uniform, 1, &value);
}

void Shader::setUniform(const std::string &name, const int &value)
{
	GLuint uniform;
	if (getUniform(name, uniform)) glUniform1i(uniform, value);
}

void Shader::setUniform(const std::string &name, const unsigned int &value)
{
	GLuint uniform;
	if (getUniform(name, uniform)) glUniform1i(uniform, value);
}

void Shader::setUniform(const std::string &name, const bool &value)
{
	GLuint uniform;
	if (getUniform(name, uniform)) glUniform1i(uniform, value);
}