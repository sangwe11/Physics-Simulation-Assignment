#include "PhysicsMaterial.h"
#include "../Core/Engine.h"

PhysicsMaterial::PhysicsMaterial(const float &static_coeff, const float &dynamic_coeff, const float &restituion)
{
	m_material = Engine::instance().physics().get_physx()->createMaterial(static_coeff, dynamic_coeff, restituion);
}