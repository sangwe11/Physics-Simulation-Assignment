#include "PhysicsCharacter.h"

#include "Math.h"

PhysicsCharacter::PhysicsCharacter(physx::PxController* controller)
{
	m_controller = controller;
	m_transform.position(physx::to_glm(m_controller->getPosition()));
}

const glm::vec3 PhysicsCharacter::position() const
{
	return physx::to_glm(m_controller->getActor()->getGlobalPose().p);
}

Transform &PhysicsCharacter::transform()
{
	m_transform.position(physx::to_glm(m_controller->getPosition()));

	return m_transform;
}

void PhysicsCharacter::move(const glm::vec3 &target, glm::quat orientation, const float &delta_time)
{
	transform();

	if (!m_grounded)
	{
		m_velocity += glm::vec3(0.0f, -9.81f, 0.0f) * delta_time;
	}

	const physx::PxU32 flags = m_controller->move(glm::to_physx(target + (m_velocity * delta_time)), 0.01f, delta_time, m_filters);

	if (flags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_grounded = true;
		m_velocity = glm::vec3(0.0f, -9.81f, 0.0f);
	}
	else
	{
		m_grounded = false;
	}

	//physx::PxTransform pose = m_controller->getActor()->getGlobalPose();
	//pose.q = glm::to_physx(orientation);
	//m_controller->getActor()->setGlobalPose(pose);
}

PhysicsCharacterDynamic::PhysicsCharacterDynamic(const Transform &transform, const float &height, const float &radius, const float &density, const PhysicsMaterial &material)
: PhysicsDynamicActor(transform)
{
	Transform local_pose(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f));
	add_geometry(physx::PxCapsuleGeometry(radius, height / 2.0f), material, density, glm::to_physx(local_pose));
	get()->setName("Player");
	//make_kinematic(true);

	set_density(density);
}

void PhysicsCharacterDynamic::move(glm::vec3 target_vel)
{
	glm::vec3 current_vel = physx::to_glm(get()->getLinearVelocity());
	glm::vec3 change_vel = target_vel - current_vel;
	glm::vec3 final_vel = current_vel + change_vel;
	final_vel.y = current_vel.y;

	get()->setLinearVelocity(glm::to_physx(final_vel));
	get()->setAngularVelocity(glm::to_physx(glm::vec3(0.0f)));

	Transform pose = transform();
	pose.orientation(glm::quat());
	get()->setGlobalPose(glm::to_physx(pose));

	get()->getRigidBodyFlags();
}

void PhysicsCharacterDynamic::jump()
{
	get()->addForce(physx::PxVec3(0.0f, 250.0f, 0.0f), physx::PxForceMode::eIMPULSE);
}