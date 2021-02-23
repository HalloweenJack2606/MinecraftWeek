#pragma once
#include <glm/glm.hpp>
#include "World.h"

class Ray
{
private:
	glm::vec3 m_RayStart = glm::vec3(0.0f);
	glm::vec3 m_RayEnd = glm::vec3(0.0f);
	glm::vec3 m_Direction = glm::vec3(0.0f);
public:
	Ray(const glm::vec3& position, const glm::vec3& direction);
	void Update();
	const glm::vec3& GetEnd() const;
	float GetLength() const;
};