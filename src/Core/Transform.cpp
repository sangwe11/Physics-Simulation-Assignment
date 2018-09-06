#include "Transform.h"

#include <sstream>

Transform::Transform(const glm::vec3 &position, const glm::vec3 &eulers, const glm::vec3 &scale)
{
	m_position = position;
	m_scale = scale;

	// Rotate into position
	rotateLocal(eulers.z, glm::vec3(0.0f, 0.0f, 1.0f));
	rotateLocal(eulers.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotateLocal(eulers.x, glm::vec3(1.0f, 0.0f, 0.0f));
}

Transform::Transform(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &scale)
{
	m_position = position;
	m_orientation = orientation;
	m_scale = scale;

	recalculateAxis();
}

const glm::vec3 Transform::global_position() const
{
	if (m_parent == nullptr)
	{
		return m_position;
	}
	else
	{
		return glm::vec3(m_parent->modelMatrix() * glm::vec4(m_position, 1.0));
	}
}

const glm::quat Transform::global_orientation() const
{
	if (m_parent == nullptr)
	{
		return m_orientation;
	}
	else
	{
		return glm::quat_cast(m_parent->modelMatrix()) * m_orientation;
	}
}

const glm::vec3 Transform::global_scale() const
{
	if (m_parent == nullptr)
	{
		return m_scale;
	}
	else
	{
		return  m_parent->scale() * m_scale;
	}
}

const glm::vec3 Transform::global_xaxis() const
{
	if (m_parent == nullptr)
	{
		return m_xaxis;
	}
	else
	{
		return glm::normalize(glm::vec3(m_parent->modelMatrix() * glm::vec4(m_xaxis, 1.0)));
	}
}

const glm::vec3 Transform::global_yaxis() const
{
	if (m_parent == nullptr)
	{
		return m_yaxis;
	}
	else
	{
		return glm::normalize(glm::vec3(m_parent->modelMatrix() * glm::vec4(m_yaxis, 1.0)));
	}
}

const glm::vec3 Transform::global_zaxis() const
{
	if (m_parent == nullptr)
	{
		return m_zaxis;
	}
	else
	{
		return glm::normalize(glm::vec3(m_parent->modelMatrix() * glm::vec4(m_zaxis, 1.0)));
	}
}

void Transform::rotateLocal(const float &amount, const glm::vec3 &axis, const bool &inRadians)
{
	m_orientation = m_orientation * glm::angleAxis((inRadians ? amount : glm::radians(amount)), axis);

	recalculateAxis();
}

void Transform::rotateGlobal(const float &amount, const glm::vec3 &axis, const bool &inRadians)
{
	m_orientation = glm::angleAxis((inRadians ? amount : glm::radians(amount)), axis) * m_orientation;

	recalculateAxis();
}

const glm::mat4 Transform::modelMatrix() const
{
	glm::mat4 model_matrix = glm::mat4();

	if (m_parent != nullptr)
	{
		model_matrix = m_parent->modelMatrix();
	}

	return model_matrix * glm::translate(m_position) * glm::mat4_cast(m_orientation) * glm::scale(m_scale);
}

const glm::mat4 Transform::invModelMatrix() const
{
	return glm::inverse(modelMatrix());
}

void Transform::recalculateAxis()
{
	glm::mat3 orient = glm::mat3_cast(m_orientation);

	// Calculate local axis
	m_xaxis = glm::normalize(orient * glm::vec3(1.0f, 0.0f, 0.0f));
	m_yaxis = glm::normalize(orient * glm::vec3(0.0f, 1.0f, 0.0f));
	m_zaxis = glm::normalize(orient * glm::vec3(0.0f, 0.0f, 1.0f));
}

const std::string Transform::to_string()
{
	std::stringstream str;

	str << "Transform(pos{" << m_position.x << ", " << m_position.y << ", " << m_position.z << "}, orientation{" << m_orientation.w << ", " << m_orientation.x << ", " << m_orientation.y << ", " << m_orientation.z << "}, scale{" << m_scale.x << ", " << m_scale.y << ", " << m_scale.z << "});";

	return str.str();
}