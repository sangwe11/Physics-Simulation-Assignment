#include "Light.h"

#include <iostream>
#include <vector>

#include <ASSIMP/mesh.h>
#include <ASSIMP/scene.h>
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/postprocess.h>

LightMesh::LightMesh(const std::string& file)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes);
	
	if (!scene)
	{
		std::cout << "Error loading mesh: " << file << std::endl;
	}
	else
	{
		std::cout << "Loaded model file: " << file << std::endl;

		if (scene->mMeshes[0] != nullptr)
		{

			// Create mesh
			std::vector<unsigned int> indices;
			std::vector<glm::vec3> vertices;

			for (unsigned int i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
			{
				indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[0]);
				indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[1]);
				indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[2]);
				drawCount += 3;
			}

			glGenBuffers(1, &indexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			for (unsigned int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
				vertices.push_back(glm::vec3(scene->mMeshes[0]->mVertices[i].x, scene->mMeshes[0]->mVertices[i].y, scene->mMeshes[0]->mVertices[i].z));

			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

			glGenVertexArrays(1, &vaObject);

		}
		else
		{
			std::cout << "Scene doesn't contain any meshes." << std::endl;
		}

	}
	
	// Free scene
	importer.FreeScene();

}

LightMesh::~LightMesh()
{
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vaObject);	
}

void Light::Init()
{
}

void LightMesh::Setup(Shader& shader)
{
	glBindVertexArray(vaObject);

	GLuint vertexAttrib;

	if(shader.getAttrib("vertex", vertexAttrib))
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glEnableVertexAttribArray(vertexAttrib);
		glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	setup = true;
}

PointLight::PointLight(glm::vec3 color, float intensity, float ambient, float radius, float falloff)
{
	this->color = color;
	this->intensity = intensity;
	this->ambient = ambient;
	this->radius = radius;
	this->falloff = falloff;

	mesh = new LightMesh("models/lighting/sphere.obj");
}

PointLight::~PointLight()
{
	delete mesh;	
}

/*SpotLight::SpotLight(glm::vec3 color, float intensity, float ambient, float radius, float falloff, float coneAngle, glm::vec3 coneDirection)
{
	this->color = color;
	this->intensity = intensity;
	this->ambient = ambient;
	this->radius = radius;
	this->falloff = falloff;
	this->coneAngle = coneAngle;
	this->coneDirection = coneDirection;

	mesh = new LightMesh(CONFIG_MODELS_DIR + "lighting/cone.obj");

}

SpotLight::~SpotLight()
{
	delete mesh;	
}*/

DirectionalLight::DirectionalLight(glm::vec3 color, float intensity, float ambient, glm::vec3 direction)
{
	this->color = color;
	this->intensity = intensity;
	this->ambient = ambient;
	this->direction = direction;

	mesh = new LightMesh("models/lighting/quad.obj");
}

DirectionalLight::~DirectionalLight()
{
	delete mesh;	
}