#ifndef _LIGHT_H
#define _LIGHT_H

#include <GLM/glm.hpp>

#include "Shader.h"

// Struct to load and hold a mesh for rendering lights
// Sphere is used for point light, cone for spotlights and a basic quad for directional lights
struct LightMesh
{
	LightMesh(const std::string& file);
	~LightMesh();
	void Setup(Shader& shader);

	unsigned int drawCount = 0;
	GLuint indexBuffer;
	GLuint vertexBuffer;
	GLuint vaObject;
	GLuint vertexAttrib;
	bool setup = false;
};

struct Light
{
	glm::vec3 color;
	float intensity;
	float ambient;

	void Init();

	LightMesh* mesh = nullptr;
};

struct PointLight : public Light
{
	PointLight(glm::vec3 color, float intensity, float ambient, float radius, float falloff);
	~PointLight();

	// Radius and fall off used over 3 attenuation values to provide a more intuative way of setting up lights
	float radius;
	float falloff;
};

/*struct SpotLight : public Light
{
	SpotLight(glm::vec3 color, float intensity, float ambient, float radius, float falloff, float coneAngle, glm::vec3 coneDirection);
	~SpotLight();
	
	float radius;
	float falloff;
	float coneAngle;
	glm::vec3 coneDirection;
};*/

struct DirectionalLight : public Light
{
	DirectionalLight(glm::vec3 color, float intensity, float ambient, glm::vec3 direction);
	~DirectionalLight();

	glm::vec3 direction;
};

#endif