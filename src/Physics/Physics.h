#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <PhysX/PxPhysicsAPI.h>
#include <GLM/glm.hpp>
#include <iostream>

class PhysicsError : public physx::PxErrorCallback
{
public:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		std::cout << message << std::endl;
	}
};

class Physics
{
public:
	Physics();
	~Physics();

	Physics(Physics const &) = delete;
	void operator=(Physics const &) = delete;

	physx::PxPhysics *get_physx(){ return m_physics; }
	physx::PxFoundation *get_foundation(){ return m_foundation; }
	physx::PxCooking *get_cooking() { return m_cooking; }

private:
	PhysicsError m_error_callback;

	physx::PxDefaultAllocator m_allocator;
	physx::PxFoundation *m_foundation;
	physx::PxPhysics *m_physics;
	physx::PxCooking *m_cooking;

	physx::debugger::comm::PvdConnection *m_vpd;
};

#endif