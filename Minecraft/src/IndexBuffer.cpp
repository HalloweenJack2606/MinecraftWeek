#include "IndexBuffer.h"

void IndexBuffer::Generate()
{
	glGenBuffers(1, &m_IBO);
}

void IndexBuffer::Init()
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_INDICES_SIZE, nullptr, GL_DYNAMIC_DRAW);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Destroy()
{
	glDeleteBuffers(1, &m_IBO);
	m_IBO = 0;
}
