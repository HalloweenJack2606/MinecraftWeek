#include "GUI.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Resources.h"

GUI::GUI(const glm::vec2 position)
{
	m_Shader.Compile("#version 330 core\n"
		"layout (location = 0) in vec2 pos;\n"
		"out vec2 oUV;\n"
		"uniform mat4 model;\n"
		"void main()\n"
		"{\n"
		"gl_Position = model * vec4(pos, 0.0, 1.0);\n"
		"oUV = vec2((pos.x + 1.0) / 2.0, 1 - (pos.y + 1.0) / 2.0);\n"
		"}\0",

		"#version 330 core\n"
		"in vec2 oUV;\n"
		"out vec4 fragColor;\n"
		"uniform sampler2D uTexture;\n"
		"void main()\n"
		"{\n"
		"vec4 t = texture(uTexture, vec2(oUV.x, oUV.y)); \n"
		"fragColor = t;\n"
		"}\0");

	this->position = position;

	float vertices[] = {
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f, -1.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GUI::~GUI()
{
	m_Shader.Destroy();
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	m_VAO = 0;
	m_VBO = 0;
}

void GUI::Render()
{
	glDisable(GL_DEPTH_BUFFER);
	Resources::GetTexture("crosshair").Bind();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_Shader.Use();
	m_Shader.SetMat4("model", model);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_BUFFER);
}