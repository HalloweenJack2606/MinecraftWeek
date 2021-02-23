#pragma once
#include "RenderDefines.h"

class IndexBuffer
{
private:
	unsigned int m_IBO = 0;
public:
	void Generate();
	void Init();
	void Bind();
	void Unbind();
	void Destroy();

	inline const bool IsEmpty() const { return m_IBO; }
};

