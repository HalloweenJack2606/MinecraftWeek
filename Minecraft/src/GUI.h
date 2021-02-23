#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class GUI
{
public:
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.014f, 0.025f);
private:
	Shader m_Shader;
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
public:
	GUI(const glm::vec2 position);
	~GUI();
	void Render();
};

