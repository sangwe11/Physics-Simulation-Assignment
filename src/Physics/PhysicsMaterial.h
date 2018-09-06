#ifndef _PHYSICSMATERIAL_H
#define _PHYSICSMATERIAL_H

#include "Physics.h"

class PhysicsMaterial
{
public:
	PhysicsMaterial(const float &static_coeff, const float &dynamic_coeff, const float &restituion);

	physx::PxMaterial* get(){ return m_material; }
	const physx::PxMaterial* get() const { return m_material; }

private:
	glm::vec3 m_colour;

	physx::PxMaterial* m_material;
};

#endif