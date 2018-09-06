#include "Object.h"

#include "Engine.h"

#include <iostream>

void NObject::axis_draw(const Camera &camera, const float &scale)
{
	Mesh &axis = Engine::instance().basic_mesh(MeshPrimiative::AXIS);
	Shader &axis_shader = Engine::instance().shader("shaders/physx_axis_debug");

	axis_shader.use();
	axis_shader.setUniform("model", transform().modelMatrix());
	axis_shader.setUniform("view_proj", camera.view_projection());
	axis_shader.setUniform("axis_scale", glm::scale(glm::vec3(scale)));

	axis.bind();

	// X axis
	axis_shader.setUniform("axis_rotation", glm::mat4());
	axis_shader.setUniform("axis_color", glm::vec3(1.0f, 0.0f, 0.0f));
	axis.draw();

	// Y axis
	axis_shader.setUniform("axis_rotation", glm::axisAngleMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(90.0f)));
	axis_shader.setUniform("axis_color", glm::vec3(0.0f, 1.0f, 0.0f));
	axis.draw();

	// Z axis
	axis_shader.setUniform("axis_rotation", glm::axisAngleMatrix(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(90.0f)));
	axis_shader.setUniform("axis_color", glm::vec3(0.0f, 0.0f, 1.0f));
	axis.draw();

	axis.unbind();
}

void NRenderedObject::add_model(const std::string &model_file, const Shader &shader, const Transform &local_transform)
{
	// Preload model
	Engine::instance().load_model(model_file, shader);

	// Store saved model name
	m_models.emplace_back(shader.name() + model_file);
	m_model_transforms.emplace_back(local_transform);
}

void NRenderedObject::draw(const Camera &camera)
{
	for (unsigned int i = 0; i < m_models.size(); ++i)
	{
		Model &model = Engine::instance().load_model(m_models[i]);
		
		model.shader().use();

		// Set shader uniforms
		model.shader().setUniform("view_proj", camera.view_projection());
		model.shader().setUniform("model", transform().modelMatrix() * m_model_transforms[i].modelMatrix());

		// Draw
		model.draw();
	}
}

NNonPhysicsObject::NNonPhysicsObject(const Transform &t)
: m_transform(t)
{

}

const Transform &NNonPhysicsObject::transform()
{
	return m_transform;
}

void NNonPhysicsObject::transform(const Transform &t)
{
	m_transform = t;
}

void NNonPhysicsObject::debug_draw(const Camera &camera)
{

}

const Transform &NPhysicsObject::transform()
{
	return actor().transform();
}

void NPhysicsObject::transform(const Transform &t)
{
	actor().transform(t);
}

void NPhysicsObject::axis_draw(const Camera &camera, const float &scale)
{
	NObject::axis_draw(camera, scale * actor().get()->getWorldBounds().getDimensions().magnitude());
}

void NPhysicsObject::debug_draw(const Camera &camera)
{
	actor().draw(camera);
}

NStaticObject::NStaticObject(const Transform &t)
{
	m_actor = std::make_unique<PhysicsStaticActor>(t);
}

PhysicsStaticActor &NStaticObject::actor()
{
	return *m_actor;
}

NDynamicObject::NDynamicObject(const Transform &t)
{
	m_actor = std::make_unique<PhysicsDynamicActor>(t);
}

PhysicsDynamicActor &NDynamicObject::actor()
{
	return *m_actor;
}