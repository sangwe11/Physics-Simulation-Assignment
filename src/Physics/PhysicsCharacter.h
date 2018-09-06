#ifndef _PHYSICSCHARACTER_H
#define _PHYSICSCHARACTER_H

#include "Physics.h"
#include "PhysicsActor.h"
#include "PhysicsMaterial.h"
#include "../Core/Transform.h"
#include "../Core/Time.h"

class PhysicsCharacterCallback : public physx::PxUserControllerHitReport
{
public:
	void onShapeHit(const physx::PxControllerShapeHit &hit)
	{
		// Add down force to simulate the players weight to dynamic objects
		physx::PxRigidDynamic *dynamic_actor = hit.actor->isRigidDynamic();
		physx::PxController *controller = hit.controller;

		if (dynamic_actor != nullptr && !(dynamic_actor->getRigidDynamicFlags() & physx::PxRigidBodyFlag::eKINEMATIC))
		{
			//glm::vec3 hit_dir = physx::to_glm(hit.dir);
			//Transform controller_transform = physx::to_glm(hit.controller->getActor()->getGlobalPose());

			//glm::vec4 dir = controller_transform.modelMatrix() * glm::vec4(hit_dir, 0.0f);
			//glm::vec3 final_dir = glm::vec3(dir);

			physx::PxVec3 force = hit.dir * hit.controller->getActor()->getMass() * hit.length;

			std::cout << glm::to_string(physx::to_glm(force)) << std::endl;

			physx::PxVec3 global_pos = physx::PxVec3(hit.worldPos.x, hit.worldPos.y, hit.worldPos.z);

			force.y *= 9.81f;

			physx::PxRigidBodyExt::addForceAtPos(*dynamic_actor, force, global_pos, physx::PxForceMode::eFORCE);
		}
	}

	void onControllerHit(const physx::PxControllersHit &hit)
	{

	}

	void onObstacleHit(const physx::PxControllerObstacleHit &hit)
	{

	}
};

class PhysicsCharacterBehaviourCallback : public physx::PxControllerBehaviorCallback
{
public:
	physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxShape& shape, const physx::PxActor& actor)
	{
		const physx::PxRigidDynamic *dynamic_actor = actor.isRigidDynamic();
		const physx::PxRigidStatic *static_actor = actor.isRigidStatic();

		if (dynamic_actor != nullptr && (dynamic_actor->getRigidDynamicFlags() & physx::PxRigidBodyFlag::eKINEMATIC))
		{
			return physx::PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
		}

		if (dynamic_actor != nullptr)
		{
			//shape.normal
		}

		return physx::PxControllerBehaviorFlag::eCCT_USER_DEFINED_RIDE;
	}

	physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxController& controller)
	{
		return physx::PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT | physx::PxControllerBehaviorFlag::eCCT_SLIDE;
	}

	physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxObstacle& obstacle)
	{
		return physx::PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT | physx::PxControllerBehaviorFlag::eCCT_SLIDE;
	}

private:
};

class PhysicsCharacter
{
public:
	PhysicsCharacter(physx::PxController* controller);

	Transform &transform();

	const glm::vec3 position() const;

	const bool grounded() { return m_grounded; }

	void move(const glm::vec3 &target, glm::quat orientation, const float &delta_time);

	void jump()
	{
		if (m_grounded)
		{
			m_grounded = false;
			m_velocity += glm::vec3(0.0f, 15.0f, 0.0f);
		}
	}

	physx::PxController* get() { return m_controller; }

	PhysicsDynamicActor actor() { return PhysicsDynamicActor(m_controller->getActor()); }

private:
	Transform m_transform;

	physx::PxController* m_controller;
	physx::PxControllerFilters m_filters;

	bool m_grounded = true;
	glm::vec3 m_velocity;
};

class PhysicsCharacterDynamic : public PhysicsDynamicActor
{
public:
	PhysicsCharacterDynamic(const Transform &transform, const float &height, const float &radius, const float &density, const PhysicsMaterial &material);

	void move(glm::vec3 target);
	void jump();
};

#endif