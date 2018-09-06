#ifndef _MATERIAL2D_H
#define _MATERIAL2D_H

#include <string>
#include <assimp/material.h>
#include <glm/glm.hpp>

#include "Texture2D.h"
#include "Shader.h"

class Material2D
{
public:
	enum TextureType { Ambient, Diffuse, Normal, Count };

	Material2D(aiMaterial *material, const std::string mpath);
	~Material2D();

	void use(Shader &shader);

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;

	float shininess;
	float shininessStrength;
	float opacity;

	bool ambientTexture = false;
	bool diffuseTexture = false;
	bool normalTexture = false;

	Texture2D *textures[TextureType::Count];
};

#endif