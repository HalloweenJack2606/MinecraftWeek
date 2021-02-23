#include "Ray.h"

Ray::Ray(const glm::vec3& position, const glm::vec3& direction)
    :m_RayStart(position), m_RayEnd(position), m_Direction(direction) {}

void Ray::Update()
{
    m_RayEnd += m_Direction / 2.0f;
}

const glm::vec3& Ray::GetEnd() const
{
    return m_RayEnd;
}

float Ray::GetLength() const
{
    return glm::distance(m_RayStart, m_RayEnd);
}