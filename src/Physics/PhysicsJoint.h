#ifndef _PHYSICSJOINT_H
#define _PHYSICSJOINT_H

#include "Physics.h"
#include "PhysicsActor.h"

class Joint
{
public:
	Joint()
	{

	}

	physx::PxJoint *get() { return m_joint; }

protected:
	physx::PxJoint *m_joint;
};

class FixedJoint : public Joint
{
public:
	FixedJoint(PhysicsRigidActor &actor1, Transform &frame1, PhysicsRigidActor &actor2, Transform &frame2);

	PhysicsRigidActor &actor1() { return m_actor1; }
	PhysicsRigidActor &actor2() { return m_actor2; }

private:
	PhysicsRigidActor &m_actor1;
	PhysicsRigidActor &m_actor2;
};

class DistanceJoint : public Joint
{
public:
	DistanceJoint(PhysicsRigidActor &actor1, Transform &frame1, PhysicsRigidActor &actor2, Transform &frame2, const bool &limited, const float &min, const float &max, const bool &spring, const float &stiffness, const float &damping);

	PhysicsRigidActor &actor1() { return m_actor1; }
	PhysicsRigidActor &actor2() { return m_actor2; }

	void stiffness(const float &val);
	const float stiffness() const;

	void damping(const float &val);
	const float damping() const;

private:
	PhysicsRigidActor &m_actor1;
	PhysicsRigidActor &m_actor2;
};

class HingeJoint : public Joint
{
public:
	HingeJoint(PhysicsRigidActor &actor1, Transform &frame1, PhysicsRigidActor &actor2, Transform &frame2);

	PhysicsRigidActor &actor1() { return m_actor1; }
	PhysicsRigidActor &actor2() { return m_actor2; }

private:
	PhysicsRigidActor &m_actor1;
	PhysicsRigidActor &m_actor2;
};

#endif