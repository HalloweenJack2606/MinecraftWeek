#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
private:
	unsigned int m_ID;
public:
	void Compile(const char* vs, const char* fs);
	Shader& Use();
	void Destroy();

	void SetMat4(const std::string& name, const glm::mat4& mat) const;
private:
	void CheckErrors(const unsigned int& shader, const char* name);
};

