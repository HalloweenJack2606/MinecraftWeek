#include "Camera.h"

Camera::Camera(World* world)
	:m_World(world) 
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), (float)m_Viewport.x / (float)m_Viewport.y, m_Near, m_Far);
}

void Camera::Update()
{
	m_FOV -= (float)Input::s_ScrollY;
	if (m_FOV < 1.0f) m_FOV = 1.0f;
	if (m_FOV > 45.0f) m_FOV = 45.0f;

	m_ViewMatrix = CalculateView();

	m_World->SetPV(m_ProjectionMatrix * m_ViewMatrix);

	glm::vec3 direction;
	direction.x = cos(glm::radians(Input::s_Yaw)) * cos(glm::radians(Input::s_Pitch));
	direction.y = sin(glm::radians(Input::s_Pitch));
	direction.z = sin(glm::radians(Input::s_Yaw)) * cos(glm::radians(Input::s_Pitch));
	front = glm::normalize(direction);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	float velocity = speed * Time::deltaTime;

	if (Input::GetKeyDown(Key_D)) this->position += right * velocity;
	if (Input::GetKeyDown(Key_A)) this->position -= right * velocity;
	if (Input::GetKeyDown(Key_W)) this->position += front * velocity;
	if (Input::GetKeyDown(Key_S)) this->position -= front * velocity;
	if (Input::GetKeyDown(Key_Space)) this->position.y += velocity;
	if (Input::GetKeyDown(Key_LeftShift)) this->position.y -= velocity;

	Ray ray(position, direction);
	for (; ray.GetLength() < 8; ray.Update())
	{
		auto rounded = glm::vec3(round(ray.GetEnd()));
		auto position = m_World->PositionToChunk(rounded);
		if (auto pChunk = m_World->GetChunk(position))
		{
			if (pChunk->GetData(rounded))
			{
				m_LastPosition = rounded;
				break;
			}
		}
	}
	if (Input::GetMouseUp(Key_Mouse0)) m_World->BlockRemove(m_LastPosition, direction);
	if (Input::GetMouseUp(Key_Mouse1)) m_World->BlockPlace(m_LastPosition, direction);
}

glm::mat4 Camera::CalculateView()
{
	return glm::lookAt(position, position + front, up);
}