#ifndef _BASICACTORS_H
#define _BASICACTORS_H

#include "Core/Object.h"
#include "Physics/PhysicsActor.h"
#include "Physics/PhysicsMaterial.h"

class Plane : public PhysicsStaticActor
{
public:
	Plane(const glm::vec3 &normal, const PhysicsMaterial &material);
};

class Box : public PhysicsDynamicActor
{
public:
	Box(const Transform &transform, const PhysicsMaterial &material, const glm::vec3 dimmensions, float density);
};

class Crate1 : public NDynamicObject
{
public:
	Crate1(const glm::vec3 &position, const float &scale, const Shader &shader, const PhysicsMaterial &material)
		: NDynamicObject(Transform(position))
	{
		add_model("models/Environment/Crates/Crate_mossy.obj", shader, Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(scale)));
		actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(1.0f) * scale)), material, 10.0f);
	}
};

class Crate2 : public NDynamicObject
{
public:
	Crate2(const glm::vec3 &position, const float &scale, const Shader &shader, const PhysicsMaterial &material)
		: NDynamicObject(Transform(position))
	{
		add_model("models/Environment/Crates/Crate_wooden.obj", shader, Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(scale)));
		actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(1.0f) * scale)), material, 10.0f);
	}
};

class Barrel : public NDynamicObject
{
public:
	Barrel(const glm::vec3 &position, const float &scale, const Shader &shader, const PhysicsMaterial &material)
		: NDynamicObject(Transform(position, glm::vec3(0.0f, 0.0f, 90.0f)))
	{
		add_model("models/Props/Drum/drum.obj", shader, Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(scale)));
		actor().add_geometry(physx::PxCapsuleGeometry(0.34f * scale, 0.23f * scale), material, 10.0f);
		actor().add_geometry(physx::PxBoxGeometry(physx::PxVec3(0.1f, 0.2f, 0.2f) * scale), material, 10.0f, physx::PxTransform(physx::PxVec3(-0.48f, 0.0f, 0.0f) * scale));
		actor().add_geometry(physx::PxBoxGeometry(physx::PxVec3(0.1f, 0.2f, 0.2f) * scale), material, 10.0f, physx::PxTransform(physx::PxVec3(0.48f, 0.0f, 0.0f) * scale));
	}
};

class Brick : public NDynamicObject
{
public:
	Brick(const glm::vec3 &position, const float &scale, const Shader &shader, const PhysicsMaterial &material)
		: NDynamicObject(Transform(position))	
	{
		add_model("models/Props/Brick/lego_brick.obj", shader, Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.1f) * scale));
		actor().add_geometry(physx::PxBoxGeometry(glm::to_physx(glm::vec3(0.1f, 0.074f, 0.24f) * scale)), material, 100.0f); //1175
	}
};

class Domino : public NDynamicObject
{
public:
	Domino(const glm::vec3 &position, const float &angle, const float &scale, const Shader &shader, const PhysicsMaterial &material)
		: NDynamicObject(Transform(position, glm::vec3(0.0f, angle, 0.0f)))
	{
		add_model("models/Props/Domino/domino.obj", shader, Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f) * scale));
		actor().add_geometry(physx::PxBoxGeometry(physx::PxVec3(1.0f, 2.0f, 0.2f) * scale), material, 10.0f);
	}

};

/*class MovingPlatform : public PhysicsDynamicObject
{
public:
	MovingPlatform(const glm::vec3 &position, Shader& shader, const glm::vec3 &direction, const float &distance, const float &speed, const float &start, const PhysicsMaterial &material)
		: PhysicsDynamicObject("models/Environment/platform.obj", shader, Transform(position, glm::vec3(0.0f), glm::vec3(2.0f, 1.0f, 2.0f)))
	{
		m_progress = start;
		m_speed = speed;
		m_start = position;
		m_direction = direction;
		m_distance = distance;

		m_actor.add_geometry(physx::PxBoxGeometry(physx::PxVec3(2.0f, 0.1f, 2.0f)), material, 10.0f, physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)));
		m_actor.make_kinematic(true);
	}

	void update(float delta_time)
	{
		if (delta_time > 1.0f)
			delta_time = 1.0f;

		if (m_delay > 0.0f)
		{
			m_delay -= delta_time;
			return;
		}

		// Moving platform
		glm::vec3 movement = glm::slerp(glm::vec3(0.0f), m_direction * m_distance, m_progress);

		m_progress += delta_time * m_speed;

		Transform t = m_actor.transform();
		t.position(m_start + movement);
		m_actor.get()->setKinematicTarget(glm::to_physx(t));
	}

private:
	// Moving platform
	float m_speed;
	glm::vec3 m_start;
	glm::vec3 m_direction;
	float m_distance;

	float m_direction_mult = 1.0f;
	float m_progress = 0.0f;
	float m_delay = 0.0f;

};*/

#endif