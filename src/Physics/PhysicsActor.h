#ifndef _PHYSICSACTOR_H
#define _PHYSICSACTOR_H

#include <memory>
#include <map>

#include "Physics.h"
#include "PhysicsMaterial.h"

#include "../Core/Transform.h"
#include "../Core/Math.h"
#include "../Rendering/Model.h"
#include "../Rendering/Camera.h"

class PhysicsActor
{
public:
	PhysicsActor(const Transform &transform = Transform());

	PhysicsActor(PhysicsActor const &) = delete;
	void operator=(PhysicsActor const &) = delete;

	virtual physx::PxActor* get() { return m_actor; }

	void name(const std::string &name);
	const std::string name() { return m_name; }

	virtual void add_geometry(const physx::PxGeometry& geom, const PhysicsMaterial &material, const physx::PxTransform &pose = physx::PxTransform::createIdentity()) = 0;

	virtual Transform &transform() = 0;
	virtual void transform(const Transform &t) = 0;

	virtual void draw(const Camera &camera) = 0;

protected:
	physx::PxActor* m_actor;
	std::string m_name = "";

	Transform m_transform;

	std::map<physx::PxShape *, std::unique_ptr<Mesh>> m_meshes;
	//std::vector<Transform> m_mesh_transforms;
};

class PhysicsRigidActor : public PhysicsActor
{
public:
	PhysicsRigidActor(const Transform &transform = Transform());

	void add_geometry(const physx::PxGeometry& geom, const PhysicsMaterial &material, const physx::PxTransform &pose = physx::PxTransform::createIdentity());

	Transform &transform();
	void transform(const Transform &t);

	void draw(const Camera &camera);

	virtual physx::PxRigidActor* get() { return static_cast<physx::PxRigidActor*>(m_actor); }
};

class PhysicsStaticActor : public PhysicsRigidActor
{
public:
	PhysicsStaticActor(const Transform &transform = Transform());
	PhysicsStaticActor(const physx::PxTransform &transform);
	PhysicsStaticActor(physx::PxRigidStatic* actor);

	virtual physx::PxRigidStatic* get() { return static_cast<physx::PxRigidStatic*>(m_actor); }
};

class PhysicsDynamicActor : public PhysicsRigidActor
{
public:
	PhysicsDynamicActor(const Transform &transform = Transform());
	PhysicsDynamicActor(const physx::PxTransform &transform);
	PhysicsDynamicActor(physx::PxRigidDynamic* actor);

	virtual physx::PxRigidDynamic* get() { return static_cast<physx::PxRigidDynamic*>(m_actor); }

	void make_kinematic(const bool val);

	void set_density(const float &density);

	void add_geometry(const physx::PxGeometry& geom, const PhysicsMaterial &material, const float &density, const physx::PxTransform &pose = physx::PxTransform::createIdentity());
};

#endif