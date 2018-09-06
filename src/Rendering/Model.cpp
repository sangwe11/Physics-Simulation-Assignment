#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>

Model::Model(const std::string &name, const Shader &shader) :
	m_shader(shader)
{
	// ASSIMP for model loading provides greater support
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(name, aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_TransformUVCoords | aiProcess_OptimizeMeshes);

	if (scene == nullptr)
	{
		// Couldn't load file
		std::cout << importer.GetErrorString() << std::endl;
		return;
	}

	// Work out the model path
	std::string modelPath = name.substr(0, name.find_last_of("\\/"));
	modelPath.append("/");

	// Load meshes
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		// Assimp doesn't seem to load names very well..
		std::string meshName = name + "_mesh_" + std::to_string(i);
		scene->mMeshes[i]->mName = meshName.c_str();

		// Load mesh
		Mesh *mesh = new Mesh(scene->mMeshes[i], m_shader);
		meshes.push_back(mesh);
	}

	// Load materials
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		// Assimp doesn't seem to load names very well..
		std::string materialName = name + "_material2D_" + std::to_string(i);

		// Load material
		Material2D *material = new Material2D(scene->mMaterials[i], modelPath);
		materials.push_back(material);
	}

	// Free scene
	importer.FreeScene();

	std::cout << "Loaded model: " << name << std::endl;
}

Model::~Model()
{
	for (Mesh *m : meshes)
	{
		delete m;
	}

	for (Material2D *m : materials)
	{
		delete m;
	}

	// Clear lists
	meshes.clear();
	materials.clear();
}

void Model::draw()
{
	// Draw each mesh
	for (Mesh *mesh : meshes)
	{
		// Use material
		if (materials.size() > mesh->getMaterial())
			materials[mesh->getMaterial()]->use(m_shader);

		// Update mesh uniforms
		//shader.setUniform("hasTangents", mesh->hasTangents);

		// Draw
		mesh->bind();
		glDrawElements(GL_TRIANGLES, mesh->getDrawCount(), GL_UNSIGNED_INT, 0);
		mesh->unbind();
	}
}