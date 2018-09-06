#include "Math.h"

namespace physx
{
	const glm::vec3 to_glm(const physx::PxVec3 &v)
	{
		return glm::vec3(v.x, v.y, v.z);
	}

	const glm::vec3 to_glm(const physx::PxExtendedVec3 &v)
	{
		return glm::vec3(v.x, v.y, v.z);
	}

	const glm::quat to_glm(const physx::PxQuat &q)
	{
		return glm::quat(q.w, q.x, q.y, q.z);
	}

	const Transform to_glm(const physx::PxTransform &t)
	{
		return Transform(to_glm(t.p), to_glm(t.q));
	}
}

namespace glm
{
	const physx::PxVec3 to_physx(const glm::vec3 &v)
	{
		return physx::PxVec3(v.x, v.y, v.z);
	}

	const physx::PxQuat to_physx(const glm::quat &q)
	{
		return physx::PxQuat(q.x, q.y, q.z, q.w);
	}

	const physx::PxTransform to_physx(const Transform &t)
	{
		return physx::PxTransform(to_physx(t.position()), to_physx(t.orientation()));
	}
}

namespace maths
{
	const float random(float min, float max)
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}
}