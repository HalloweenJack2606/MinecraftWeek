#include "Texture.h"

void Texture::GenerateTexture(unsigned int width, unsigned int height, BYTE* data)
{
	m_ImageFormat = GL_RGBA;
	m_InternalFormat = GL_RGBA;
	m_FilterMin = GL_NEAREST;// _MIPMAP_LINEAR;
	m_FilterMax = GL_NEAREST;
	m_WrapS = GL_REPEAT;
	m_WrapT = GL_REPEAT;
	glCreateTextures(GL_TEXTURE_2D, 1, &this->m_ID);

	this->m_Width = width;
	this->m_Height = height;
	this->m_ImageData = data;

	glBindTexture(GL_TEXTURE_2D, this->m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, this->m_Width, this->m_Height, 0, this->m_ImageFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->m_WrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->m_WrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->m_FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->m_FilterMax);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, this->m_ID);
}

void Texture::Unbind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Free()
{
	glDeleteTextures(1, &this->m_ID);
}