#include "VertexBuffer.h"

void VertexBuffer::Generate()
{
	glGenBuffers(1, &m_VBO);
}

void VertexBuffer::Init()
{
	glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Destroy()
{
	glDeleteBuffers(1, &m_VBO);
	m_VBO = 0;
}
