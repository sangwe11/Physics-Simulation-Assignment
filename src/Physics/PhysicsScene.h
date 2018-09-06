#ifndef _PHYSICSSCENE_H
#define _PHYSICSSCENE_H

#include <vector>

#include "Physics.h"
#include "PhysicsActor.h"
#include "PhysicsCharacter.h"
#include "../Core/Object.h"

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void simulate(const float &delta_time);

	virtual void init() = 0;
	virtual void update(const float &delta_time) = 0;

	void add_actor(PhysicsActor &actor);
	void remove_actor(PhysicsActor &actor);

	void add_object(NPhysicsObject &object);
	void remove_object(NPhysicsObject &object);

	std::vector<PhysicsStaticActor> get_static_actors();
	std::vector<PhysicsDynamicActor> get_dynamic_actors();

	PhysicsCharacter create_character(const float &height, const float &radius, const glm::vec3 &position, PhysicsMaterial &material, const float &density);

	bool raycast(glm::vec3 origin, glm::vec3 direction, float distance, physx::PxRaycastBuffer &hit);

	physx::PxScene* get() { return m_scene; }

protected:
	physx::PxScene* m_scene;
	physx::PxControllerManager* m_controller_manager;

	physx::PxCpuDispatcher* m_cpu_dispatch;
	physx::PxCudaContextManager *m_cuda_manager;
};

class MyScene : public PhysicsScene
{
public:
	void init()
	{

	}

	void update(const float &delta_time)
	{

	}

private:
};

#endif