#ifndef _OBJECT_H
#define _OBJECT_H

#include "Transform.h"
#include "../Rendering/Model.h"
#include "../Physics/PhysicsActor.h"
#include "../Rendering/Camera.h"

#include <GL/glew.h>
#include <vector>

class NObject
{
public:
	virtual const Transform &transform() = 0;
	virtual void transform(const Transform &t) = 0;

	virtual void draw(const Camera &camera) = 0;
	virtual void debug_draw(const Camera &camera) = 0;

	virtual void axis_draw(const Camera &camera, const float &scale);
};

class NRenderedObject : public NObject
{
public:
	void add_model(const std::string &model_file, const Shader &shader, const Transform &local_transform = Transform());
	void draw(const Camera &camera);

private:
	std::vector<std::string> m_models;
	std::vector<Transform> m_model_transforms;
};

class NNonPhysicsObject : public NRenderedObject
{
public:
	NNonPhysicsObject(const Transform &t = Transform());

	const Transform &transform();
	void transform(const Transform &t);

	void debug_draw(const Camera &camera);
	
private:
	Transform m_transform;
};

class NPhysicsObject : public NRenderedObject
{
public:
	const Transform &transform();
	void transform(const Transform &t);

	void axis_draw(const Camera &camera, const float &scale);
	void debug_draw(const Camera &camera);

	virtual PhysicsActor &actor() = 0;
	//virtual const PhysicsActor &actor() const = 0;

private:
};

class NStaticObject : public NPhysicsObject
{
public:
	NStaticObject(const Transform &t = Transform());

	PhysicsStaticActor &actor();

private:
	std::unique_ptr<PhysicsStaticActor> m_actor;
};

class NDynamicObject : public NPhysicsObject
{
public:
	NDynamicObject(const Transform &t = Transform());

	PhysicsDynamicActor &actor();

private:
	std::unique_ptr<PhysicsDynamicActor> m_actor;
};

#endif