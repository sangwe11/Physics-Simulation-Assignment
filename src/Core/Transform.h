#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Transform
{
public:
	Transform(const glm::vec3 &position = glm::vec3(0.0f), const glm::vec3 &eulers = glm::vec3(0.0f), const glm::vec3 &scale = glm::vec3(1.0f));
	Transform(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &scale = glm::vec3(1.0f));

	const glm::vec3 global_position() const;
	const glm::quat global_orientation() const;
	const glm::vec3 global_scale() const;
	const glm::vec3 global_xaxis() const;
	const glm::vec3 global_yaxis() const;
	const glm::vec3 global_zaxis() const;

	const glm::vec3 &position() const { return m_position; }
	void position(const glm::vec3 &position) { m_position = position; }

	const glm::quat &orientation() const { return m_orientation; }
	void orientation(const glm::quat &orientation) { m_orientation = orientation; }

	const glm::vec3 &scale() const { return m_scale; }
	void scale(const glm::vec3 &scale) { m_scale = scale; }

	void rotateLocal(const float &amount, const glm::vec3 &axis, const bool &inRadians = false);
	void rotateGlobal(const float &amount, const glm::vec3 &axis, const bool &inRadians = false);

	const glm::mat4 modelMatrix() const;
	const glm::mat4 invModelMatrix() const;
	const glm::vec3 &xaxis() const { return m_xaxis; }
	const glm::vec3 &yaxis() const { return m_yaxis; }
	const glm::vec3 &zaxis() const { return m_zaxis; }

	bool has_parent() { return m_parent != nullptr; }
	void parent(Transform& transform){ m_parent = &transform; }
	Transform &parent() { return *m_parent; }

	const std::string to_string();

private:
	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_scale;

	glm::vec3 m_xaxis;
	glm::vec3 m_yaxis;
	glm::vec3 m_zaxis;

	void recalculateAxis();

	Transform *m_parent = nullptr;

};

#endif