#ifndef _MESH_H
#define _MESH_H

#include "Shader.h"
#include "PhysX/PxPhysics.h"

#include <assimp/mesh.h>
#include <vector>

namespace physx
{
	std::vector<glm::vec3> physx_to_glm(const physx::PxVec3 *v, const physx::PxU32 count);
}

class Mesh
{
public:
	Mesh(aiMesh *mesh, const Shader &shader);
	Mesh(const std::vector<unsigned int> &indicies, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec2> &uvs, const Shader &shader);
	Mesh(const std::vector<glm::vec3> &vertices, const Shader &shader);
	Mesh(const std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indicies, const Shader &shader);
	~Mesh();

	const unsigned int getMaterial() { return materialIndex; }
	const unsigned int getDrawCount() { return drawCount; }

	void bind();
	void unbind();
        
    void draw();

private:
	unsigned int drawCount;
	unsigned int materialIndex;

	GLuint indexBuffer;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint tangentBuffer;
	GLuint bitangentBuffer;
	GLuint colorBuffer;
	GLuint uvBuffer;
	GLuint vaObject;

	GLuint vertexAttrib;
	GLuint normalAttrib;
	GLuint tangentAttrib;
	GLuint bitangentAttrib;
	GLuint colorAttrib;
	GLuint uvAttrib;

	bool hasNormals = false;
	bool hasTangents = false;
	bool hasBitangents = false;
	bool hasUvs = false;
};

void create_box_mesh(const glm::vec3 &half_extents, std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indicies);

#endif