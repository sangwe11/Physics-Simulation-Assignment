#include "BasicActors.h"

Plane::Plane(const glm::vec3 &normal, const PhysicsMaterial &material)
	: PhysicsStaticActor(physx::PxTransformFromPlaneEquation(physx::PxPlane(glm::to_physx(normal), 0.0f)))
{
	add_geometry(physx::PxPlaneGeometry(), material);
}

Box::Box(const Transform &transform, const PhysicsMaterial &material, const glm::vec3 dimmensions, float density)
	: PhysicsDynamicActor(transform)
{
	add_geometry(physx::PxBoxGeometry(glm::to_physx(dimmensions)), material, density);
}