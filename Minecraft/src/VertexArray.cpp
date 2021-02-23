#include "VertexArray.h"

void VertexArray::Generate()
{
	glGenVertexArrays(1, &m_VAO);
}

void VertexArray::Init()
{
	EnableAttrib(SHADER_VERTEX_POSITION);
	EnableAttrib(SHADER_UV_POSITION);

	glVertexAttribPointer(SHADER_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, RENDERER_SPRITE_SIZE, (void*)(0 * sizeof(float)));
	glVertexAttribPointer(SHADER_UV_POSITION, 3, GL_FLOAT, GL_FALSE, RENDERER_SPRITE_SIZE, (void*)(3 * sizeof(float)));
	//@note: texture uvs should be able to fit into a normalized unsigned short, which is /2 the size of a float, saving lot of space in the buffer
}

void VertexArray::EnableAttrib(std::uint16_t attrib)
{
	glEnableVertexAttribArray(attrib);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_VAO);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::Destroy()
{
	glDeleteVertexArrays(1, &m_VAO);
	m_VAO = 0;
}
