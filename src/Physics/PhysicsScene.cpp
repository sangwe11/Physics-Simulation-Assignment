#include "PhysicsScene.h"
#include "../Core/Engine.h"

#include <iostream>

PhysicsScene::PhysicsScene()
{
	physx::PxSceneDesc scene_desc(Engine::instance().physics().get_physx()->getTolerancesScale());
	scene_desc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	if (!scene_desc.cpuDispatcher)
	{
		m_cpu_dispatch = physx::PxDefaultCpuDispatcherCreate(1);

		if (!m_cpu_dispatch)
		{
			std::cerr << "Error: failed to create PhysX cpu." << std::endl;

			return;
		}

		scene_desc.cpuDispatcher = m_cpu_dispatch;
	}

	// create GPU dispatcher
    physx::PxCudaContextManagerDesc cuda_context_desc;
	m_cuda_manager = physx::PxCreateCudaContextManager(*Engine::instance().physics().get_foundation(), cuda_context_desc, nullptr);
	scene_desc.gpuDispatcher = m_cuda_manager->getGpuDispatcher();

	if (!scene_desc.filterShader)
	{
		scene_desc.filterShader = &physx::PxDefaultSimulationFilterShader;
	}

	m_scene = Engine::instance().physics().get_physx()->createScene(scene_desc);

	if (m_scene == nullptr)
	{
		std::cerr << "Error: failed to create PhysX scene." << std::endl;

		return;
	}

	m_controller_manager = PxCreateControllerManager(*m_scene);

	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCONTACT_POINT, 1.0f);
	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCONTACT_NORMAL, 1.0f);

	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_PAIRS, 1.0f);

	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 2.0f);
}

PhysicsScene::~PhysicsScene()
{
	if (m_controller_manager != nullptr)
	{
		m_controller_manager->release();
		m_controller_manager = nullptr;
	}

	if (m_scene != nullptr)
	{
		m_scene->release();
		m_scene = nullptr;
	}

	if (m_cuda_manager != nullptr)
	{
		m_cuda_manager->release();
		m_cuda_manager = nullptr;
	}


}

void PhysicsScene::simulate(const float &delta_time)
{
	m_scene->simulate(delta_time);
	m_scene->fetchResults(true);
}

void PhysicsScene::add_actor(PhysicsActor &actor)
{
	m_scene->addActor(*actor.get());
}

void PhysicsScene::remove_actor(PhysicsActor &actor)
{
	m_scene->removeActor(*actor.get());
}

void PhysicsScene::add_object(NPhysicsObject &object)
{
	m_scene->addActor(*object.actor().get());
}

void PhysicsScene::remove_object(NPhysicsObject &object)
{
	m_scene->removeActor(*object.actor().get());
}

/*std::vector<PhysicsStaticActor> PhysicsScene::get_static_actors()
{
	std::vector<physx::PxActor*> physx_actors(m_scene->getNbActors(physx::PxActorTypeSelectionFlag::eRIGID_STATIC));
	m_scene->getActors(physx::PxActorTypeSelectionFlag::eRIGID_STATIC, &physx_actors[0], (physx::PxU32)physx_actors.size());

	std::vector<PhysicsStaticActor> actors;

	for (unsigned int i = 0; i < physx_actors.size(); ++i)
	{
		actors.emplace_back(physx_actors[i]->isRigidStatic());
	}

	return actors;
}

std::vector<PhysicsDynamicActor> PhysicsScene::get_dynamic_actors()
{
	std::vector<physx::PxActor*> physx_actors(m_scene->getNbActors(physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC));
	m_scene->getActors(physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC, &physx_actors[0], (physx::PxU32)physx_actors.size());

	std::vector<PhysicsDynamicActor> actors;

	for (unsigned int i = 0; i < physx_actors.size(); ++i)
	{
		physx::PxRigidDynamic* cast = physx_actors[i]->isRigidDynamic();

		if (cast != nullptr)
		{
			actors.emplace_back(physx_actors[i]->isRigidDynamic());
		}
	}

	return actors;
}*/

PhysicsCharacter PhysicsScene::create_character(const float &height, const float &radius, const glm::vec3 &position, PhysicsMaterial &material, const float &density)
{
	physx::PxCapsuleControllerDesc controller_desc;

	controller_desc.height = height;
	controller_desc.radius = radius;
	controller_desc.position = physx::PxExtendedVec3(position.x, position.y, position.z);
	controller_desc.material = material.get();
	controller_desc.density = density;
	controller_desc.stepOffset = 0.25f;
	controller_desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
	controller_desc.callback = new PhysicsCharacterCallback();
	controller_desc.behaviorCallback = new PhysicsCharacterBehaviourCallback();
	controller_desc.maxJumpHeight = 2.0f;
	controller_desc.scaleCoeff = 0.8f;
	controller_desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	controller_desc.slopeLimit = 0.996f;
	controller_desc.invisibleWallHeight = 2.0f;
	//controller_desc.behaviorCallback.

	return PhysicsCharacter(m_controller_manager->createController(controller_desc));
}

bool PhysicsScene::raycast(glm::vec3 origin, glm::vec3 direction, float distance, physx::PxRaycastBuffer &hit)
{
	

	physx::PxQueryFilterData filter;
	filter.flags = physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC;

	return m_scene->raycast(glm::to_physx(origin), glm::to_physx(direction), distance, hit, physx::PxHitFlag::eDEFAULT, filter);

	
}