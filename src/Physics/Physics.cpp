#include "Physics.h"

#include <iostream>

Physics::Physics()
{
	// Foundation
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_error_callback);

	if (m_foundation == nullptr)
	{
		std::cerr << "Error: failed to create PhysX foundation." << std::endl;

		return;
	}

	// Physics
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale());

	if (m_physics == nullptr)
	{
		std::cerr << "Error: failed to create PhysX physics." << std::endl;

		m_foundation->release();
		return;
	}

	// Mesh cooking
	physx::PxCookingParams cooking_params(m_physics->getTolerancesScale());

	m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, cooking_params);

	if (m_cooking == nullptr)
	{
		std::cerr << "Error: failed to create PhysX mesh cooking." << std::endl;

		m_physics->release();
		m_foundation->release();

		return;
	}

	// Visual debugger
	m_vpd = physx::PxVisualDebuggerExt::createConnection(m_physics->getPvdConnectionManager(), "localhost", 5425, 100, physx::PxVisualDebuggerExt::getAllConnectionFlags());

	if (m_vpd == nullptr)
	{
		std::cerr << "Warning: PhysX visual debugger not available." << std::endl;
	}
}

Physics::~Physics()
{
	if (m_vpd != nullptr)
	{
		m_vpd->release();
		m_vpd = nullptr;
	}

	if (m_cooking != nullptr)
	{
		m_cooking->release();
		m_cooking = nullptr;
	}

	if (m_physics != nullptr)
	{
		m_physics->release();
		m_physics = nullptr;
	}

	if (m_foundation != nullptr)
	{
		m_foundation->release();
		m_foundation = nullptr;
	}
}