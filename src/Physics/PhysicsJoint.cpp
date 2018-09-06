#include "PhysicsJoint.h"
#include "../Core/Engine.h"

#include <iostream>

FixedJoint::FixedJoint(PhysicsRigidActor &actor1, Transform &frame1, PhysicsRigidActor &actor2, Transform &frame2)
: m_actor1(actor1), m_actor2(actor2)
{
	m_joint = physx::PxFixedJointCreate(*Engine::instance().physics().get_physx(), m_actor1.get()->isRigidActor(), glm::to_physx(frame1), m_actor2.get()->isRigidActor(), glm::to_physx(frame2));

	physx::PxFixedJoint* fixed_joint = static_cast<physx::PxFixedJoint*>(m_joint);

	fixed_joint->setProjectionAngularTolerance(0.1f);
	fixed_joint->setProjectionLinearTolerance(0.1f);
	fixed_joint->setConstraintFlag(physx::PxConstraintFlag::ePROJECTION, true);
	fixed_joint->setConstraintFlag(physx::PxConstraintFlag::ePROJECT_TO_ACTOR1, true);
}

DistanceJoint::DistanceJoint(PhysicsRigidActor &actor1, Transform &frame1, PhysicsRigidActor &actor2, Transform &frame2, const bool &limited, const float &min, const float &max, const bool &spring, const float &stiffness, const float &damping)
: m_actor1(actor1), m_actor2(actor2)
{
	m_joint = physx::PxDistanceJointCreate(*Engine::instance().physics().get_physx(), m_actor1.get()->isRigidActor(), glm::to_physx(frame1), m_actor2.get()->isRigidActor(), glm::to_physx(frame2));

	physx::PxDistanceJoint* distance_joint = static_cast<physx::PxDistanceJoint*>(m_joint);

	if (limited)
	{
		distance_joint->setMinDistance(min);
		distance_joint->setMaxDistance(max);
		distance_joint->setDistanceJointFlag(physx::PxDistanceJointFlag::eMIN_DISTANCE_ENABLED, true);
		distance_joint->setDistanceJointFlag(physx::PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, true);
	}

	if (spring)
	{
		distance_joint->setStiffness(stiffness);
		distance_joint->setDamping(damping);
		distance_joint->setDistanceJointFlag(physx::PxDistanceJointFlag::eSPRING_ENABLED, true);
	}

	distance_joint->setTolerance(0.0f);
}

void DistanceJoint::stiffness(const float &val)
{
	static_cast<physx::PxDistanceJoint*>(m_joint)->setStiffness(val);
}

const float DistanceJoint::stiffness() const
{
	return static_cast<physx::PxDistanceJoint*>(m_joint)->getStiffness();
}

void DistanceJoint::damping(const float &val)
{
	static_cast<physx::PxDistanceJoint*>(m_joint)->setDamping(val);
}

const float DistanceJoint::damping() const
{
	return static_cast<physx::PxDistanceJoint*>(m_joint)->getDamping();
}

HingeJoint::HingeJoint(PhysicsRigidActor &actor1, Transform &frame1, PhysicsRigidActor &actor2, Transform &frame2)
: m_actor1(actor1), m_actor2(actor2)
{
	m_joint = physx::PxRevoluteJointCreate(*Engine::instance().physics().get_physx(), m_actor1.get(), glm::to_physx(frame1), m_actor2.get(), glm::to_physx(frame2));

	physx::PxRevoluteJoint *hinge_joint = static_cast<physx::PxRevoluteJoint*>(m_joint);

	//physx::PxJointAngularLimitPair(0.0f, 0.0f);

	//physx::PxRevoluteJointFlag::eLIMIT_ENABLED

	//hinge_joint->

	hinge_joint->setProjectionAngularTolerance(0.1f);
	hinge_joint->setProjectionLinearTolerance(0.1f);
	hinge_joint->setConstraintFlag(physx::PxConstraintFlag::ePROJECTION, true);
	hinge_joint->setConstraintFlag(physx::PxConstraintFlag::ePROJECT_TO_ACTOR0, true);
	hinge_joint->setConstraintFlag(physx::PxConstraintFlag::ePROJECT_TO_ACTOR1, true);

	//physx::PxJointAngularLimitPair joint_limits(0.0f, 0.0f);
	//joint_limits.damping = 100.0f;
	//joint_limits.stiffness = 100.0f;


	//hinge_joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
	//hinge_joint->setLimit(joint_limits);

}