#pragma once
#include "RenderDefines.h"

class VertexArray
{
private:
	unsigned int m_VAO = 0;
public:
	void Generate();
	void Init();
	void EnableAttrib(std::uint16_t attrib);
	void Bind();
	void Unbind();
	void Destroy();

	inline const bool IsEmpty() const { return m_VAO; }
};

