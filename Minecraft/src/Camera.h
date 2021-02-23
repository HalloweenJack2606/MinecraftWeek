#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"
#include "World.h"
#include "Ray.h"

class World;

class Camera
{
public:
	glm::vec3 position = glm::vec3(0.0f, 80.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(0.0f);
	float speed = 10.0f;
private:
	bool m_Active = true;
	float m_Near = 0.1f;
	float m_Far = 1000.0f;
	float m_FOV = 100.0f;

	glm::vec3 m_LastPosition = glm::vec3(0.0f);
	glm::vec3 m_PointedBlock = glm::vec3(0.0f);

	glm::vec2 m_Viewport = glm::vec2(1280, 720);
	glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

	World* m_World = nullptr;
public:
	Camera(World* world);
	void Update();
private:
	glm::mat4 CalculateView();
};