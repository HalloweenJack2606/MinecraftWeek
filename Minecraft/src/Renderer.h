#pragma once
#include "Shader.h"
#include "Resources.h"

class Renderer
{
private:
	Shader m_Shader = Shader();
public:
	Renderer();
	~Renderer();

	void Prepare(const glm::mat4& pv);
};