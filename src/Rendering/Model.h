#ifndef _MODEL_H
#define _MODEL_H

#include "Shader.h"
#include "Mesh.h"
#include "Material2D.h"

#include <vector>

class Model
{
public:
	Model(const std::string &name, const Shader &shader);
	~Model();

	Shader &shader() { return m_shader; }

	void draw();

private:
	std::vector<Mesh *> meshes;
	std::vector<Material2D *> materials;

	Shader m_shader;
};

#endif