#include "Camera.h"

Camera::Camera(float fov, float near, float far, float aspect_ratio, const Transform &transform)
{
    m_transform = transform;
    m_projection = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
}
    
const glm::mat4 Camera::view() const
{
    return glm::inverse(m_transform.modelMatrix());
}

const glm::mat4 Camera::projection() const
{
    return m_projection;
}

const glm::mat4 Camera::view_projection() const
{
	return projection() * view();
}