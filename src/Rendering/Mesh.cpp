#include "Mesh.h"

#include <vector>
#include <iostream>

namespace physx
{
	std::vector<glm::vec3> physx_to_glm(const physx::PxVec3 *v, const physx::PxU32 count)
	{
		std::vector<glm::vec3> converted;

		for (unsigned int i = 0; i < count; ++i)
		{
			converted.emplace_back(v[i].x, v[i].y, v[i].z);
		}

		return converted;
	}
}

void create_box_mesh(const glm::vec3 &half_extents, std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indicies)
{
	vertices.clear();
	indicies = {0, 2, 3, 3, 1, 3, 2, 3, 4, 4, 3, 5, 4, 5, 6, 6, 5, 7, 6, 7, 0, 0, 7, 1, 1, 7, 3, 3, 7, 5, 6, 0, 4, 4, 0, 2};

	vertices.emplace_back(-half_extents.x, -half_extents.y, half_extents.z);
	vertices.emplace_back(half_extents.x, -half_extents.y, half_extents.z);
	vertices.emplace_back(-half_extents.x, half_extents.y, half_extents.z);
	vertices.emplace_back(half_extents.x, half_extents.y, half_extents.z);
	vertices.emplace_back(-half_extents.x, half_extents.y, -half_extents.z);
	vertices.emplace_back(half_extents.x, half_extents.y, -half_extents.z);
	vertices.emplace_back(-half_extents.x, -half_extents.y, -half_extents.z);
	vertices.emplace_back(half_extents.x, -half_extents.y, -half_extents.z);
}

Mesh::Mesh(aiMesh *mesh, const Shader &shader)
{
	std::vector<unsigned int> indices;
	drawCount = 0;

	// Push faces into a indicies list
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
		drawCount += 3;
	}

	// Buffer index data
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Buffer vertex data
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mVertices, GL_STATIC_DRAW);

	// Buffer normal data if present
	if (mesh->mNormals != NULL)
	{
		hasNormals = true;
		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mNormals, GL_STATIC_DRAW);

		std::cout << "Mesh " << mesh->mName.C_Str() << " has normals." << std::endl;
	}

	// Buffer tangent data if present
	if (mesh->mTangents != NULL)
	{
		hasTangents = true;
		glGenBuffers(1, &tangentBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mTangents, GL_STATIC_DRAW);

		std::cout << "Mesh " << mesh->mName.C_Str() << " has tangents." << std::endl;
	}

	// Buffer bitangent data if present
	if (mesh->mBitangents != NULL)
	{
		hasBitangents = true;
		glGenBuffers(1, &bitangentBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mBitangents, GL_STATIC_DRAW);

		std::cout << "Mesh " << mesh->mName.C_Str() << " has bitangents." << std::endl;
	}

	// Buffer uv data if present
	if (mesh->mTextureCoords != NULL)
	{
		hasUvs = true;
		glGenBuffers(1, &uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(glm::vec3), mesh->mTextureCoords[0], GL_STATIC_DRAW);

		std::cout << "Mesh " << mesh->mName.C_Str() << " has uvs." << std::endl;
	}

	// Store material index
	materialIndex = mesh->mMaterialIndex;

	// Generate vertex array and bind
	glGenVertexArrays(1, &vaObject);
	glBindVertexArray(vaObject);

	if (shader.getAttrib("vertex", vertexAttrib))
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glEnableVertexAttribArray(vertexAttrib);
		glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (shader.getAttrib("normal", normalAttrib) && hasNormals)
	{
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glEnableVertexAttribArray(normalAttrib);
		glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (shader.getAttrib("tangent", tangentAttrib) && hasTangents)
	{
		glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
		glEnableVertexAttribArray(tangentAttrib);
		glVertexAttribPointer(tangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (shader.getAttrib("bitangent", bitangentAttrib) && hasBitangents)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
		glEnableVertexAttribArray(bitangentAttrib);
		glVertexAttribPointer(bitangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (shader.getAttrib("uv", uvAttrib) && hasUvs)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glEnableVertexAttribArray(uvAttrib);
		glVertexAttribPointer(uvAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(const std::vector<unsigned int> &indicies, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec2> &uvs, const Shader &shader)
{
	drawCount = (unsigned int)indicies.size();;

	// Buffer index data
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

	// Buffer vertex data
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// Buffer uv data
	hasUvs = true;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	// Generate vertex array and bind
	glGenVertexArrays(1, &vaObject);
	glBindVertexArray(vaObject);

	if (shader.getAttrib("vertex", vertexAttrib))
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glEnableVertexAttribArray(vertexAttrib);
		glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (shader.getAttrib("uv", uvAttrib) && hasUvs)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glEnableVertexAttribArray(uvAttrib);
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(const std::vector<glm::vec3> &vertices, const Shader &shader)
{
	std::vector<unsigned int> indicies;

	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		indicies.push_back(i);
	}

	drawCount = (unsigned int)indicies.size();;

	// Buffer index data
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

	// Buffer vertex data
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// Buffer uv data
	hasUvs = false;

	// Generate vertex array and bind
	glGenVertexArrays(1, &vaObject);
	glBindVertexArray(vaObject);

	if (shader.getAttrib("vertex", vertexAttrib))
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glEnableVertexAttribArray(vertexAttrib);
		glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(const std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indicies, const Shader &shader)
{
	drawCount = (unsigned int)indicies.size();;

	// Buffer index data
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

	// Buffer vertex data
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// Buffer uv data
	hasUvs = false;

	// Generate vertex array and bind
	glGenVertexArrays(1, &vaObject);
	glBindVertexArray(vaObject);

	if (shader.getAttrib("vertex", vertexAttrib))
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glEnableVertexAttribArray(vertexAttrib);
		glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
	// Delete index and vertex buffers
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &vertexBuffer);

	// Delete other buffers
	if (hasNormals) glDeleteBuffers(1, &normalBuffer);
	if (hasTangents) glDeleteBuffers(1, &tangentBuffer);
	if (hasBitangents) glDeleteBuffers(1, &bitangentBuffer);
	if (hasUvs) glDeleteBuffers(1, &uvBuffer);

	// Delete vertex array
	glDeleteVertexArrays(1, &vaObject);
}

void Mesh::bind()
{
	glBindVertexArray(vaObject);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
}

void Mesh::draw()
{
    glBindVertexArray(vaObject);
    glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}