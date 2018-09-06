#ifndef _MATH_H
#define _MATH_H

#include "Transform.h"

#include <GLM/glm.hpp>
#include <PhysX/PxPhysicsAPI.h>

namespace physx
{

	const glm::vec3 to_glm(const physx::PxVec3 &v);
	const glm::vec3 to_glm(const physx::PxExtendedVec3 &v);
	const glm::quat to_glm(const physx::PxQuat &q);
	const Transform to_glm(const physx::PxTransform &t);

}

namespace glm
{

	const physx::PxVec3 to_physx(const glm::vec3 &v);
	const physx::PxQuat to_physx(const glm::quat &q);
	const physx::PxTransform to_physx(const Transform &t);

}

namespace maths
{
	const float random(float min, float max);
}

#endif