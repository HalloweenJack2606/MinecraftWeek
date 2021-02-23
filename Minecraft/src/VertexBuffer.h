#pragma once
#include "RenderDefines.h"

class VertexBuffer
{
private:
	unsigned int m_VBO = 0;
public:
	void Generate();
	void Init();
	void Bind();
	void Unbind();
	void Destroy();

	inline const bool IsEmpty() const { return m_VBO; }
};

