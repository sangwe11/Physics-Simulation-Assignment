#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "../Core/Transform.h"

class Camera
{
public:
    Camera(float fov, float near, float far, float aspect_ratio, const Transform &transform = Transform());

	void look(const float &x, const float &y)
	{
		if (m_lookx + x >= m_limitx_min &&  m_lookx + x <= m_limitx_max)
		{
			transform().rotateLocal(x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_lookx += x;
		}

		transform().rotateGlobal(y, glm::vec3(0.0f, 1.0f, 0.0f));
	}
    
    const glm::mat4 view() const;
	const glm::mat4 projection() const;
	const glm::mat4 view_projection() const;

	Transform &transform() { return m_transform; }
	void transform(const Transform &transform) { m_transform = transform; }
    
private:
    glm::mat4 m_projection;
    Transform m_transform;

	float m_lookx = 0.0f;
	float m_limitx_min = -89.9f;
	float m_limitx_max = 89.9f;
};

#endif

