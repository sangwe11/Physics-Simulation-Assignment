#include "Material2D.h"

#include <iostream>
#include <GLM/ext.hpp>

Material2D::Material2D(aiMaterial *material, const std::string mpath)
{
	textures[TextureType::Ambient] = nullptr;
	textures[TextureType::Diffuse] = nullptr;
	textures[TextureType::Normal] = nullptr;

	// Retrieve material names
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
	material->Get(AI_MATKEY_SHININESS, shininess);
	material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
	material->Get(AI_MATKEY_OPACITY, opacity);

	// Ambient texture
	if (material->GetTextureCount(aiTextureType_AMBIENT) > 0)
	{
		ambientTexture = true;

		// Get texture path
		aiString path;
		material->GetTexture(aiTextureType_AMBIENT, 0, &path);

		// Load texture
		textures[TextureType::Ambient] = new Texture2D(mpath + path.C_Str(), true, true, 16);
	}

	// Diffuse texture
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		diffuseTexture = true;

		// Get texture path
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		// Load texture
		textures[TextureType::Diffuse] = new Texture2D(mpath + path.C_Str(), true, true, 16);
	}

	// Normals texture
	if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
	{
		normalTexture = true;

		// Get texture path
		aiString path;
		material->GetTexture(aiTextureType_NORMALS, 0, &path);

		// Load texture
		textures[TextureType::Normal] = new Texture2D(mpath + path.C_Str(), true, true, 16);
	}

	// Height texture as normals instead?
	if (material->GetTextureCount(aiTextureType_HEIGHT) > 0 && !normalTexture)
	{
		normalTexture = true;

		// Get texture path
		aiString path;
		material->GetTexture(aiTextureType_HEIGHT, 0, &path);

		// Load texture
		textures[TextureType::Normal] = new Texture2D(mpath + path.C_Str(), true, true, 16);
	}
}

Material2D::~Material2D()
{
	for (int i = 0; i < TextureType::Count; ++i)
	{
		if (textures[i] != nullptr)
		{
			delete textures[i];
			textures[i] = nullptr;
		}
	}
}

void Material2D::use(Shader &shader)
{
	// Update material uniforms
	shader.setUniform("diffuseColor", diffuse);
	shader.setUniform("opacity", opacity);
	shader.setUniform("specularColor", specular);
	shader.setUniform("specularExponent", shininess);
	shader.setUniform("hasDiffuseMap", diffuseTexture);
	shader.setUniform("hasNormalMap", normalTexture);

	// Use diffuse texture
	if (diffuseTexture)
	{
		// Bind to texture unit 0
		textures[Material2D::TextureType::Diffuse]->use(0);

		// Set shader uniform
		shader.setUniform("diffuseMap", 0);
	}

	// Use normal texture
	if (normalTexture)
	{
		// Bind to texture unit 1
		textures[Material2D::TextureType::Normal]->use(1);

		// Set shader uniform
		shader.setUniform("normalMap", 1);
	}
}