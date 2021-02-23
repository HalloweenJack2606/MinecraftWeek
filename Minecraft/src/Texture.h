#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>

typedef std::uint8_t BYTE;

class Texture
{
private:
	unsigned int m_ID = 0;
	unsigned int m_InternalFormat = 0;
	unsigned int m_ImageFormat = 0;
	unsigned int m_WrapS = 0;
	unsigned int m_WrapT = 0;
	unsigned int m_FilterMax = 0;
	unsigned int m_FilterMin = 0;
	unsigned int m_Width = 0;
	unsigned int m_Height = 0;

	BYTE* m_ImageData = nullptr;
public:
	void GenerateTexture(unsigned int width, unsigned int height, BYTE* data);
	void Bind(unsigned int slot = 0) const;
	void Unbind(unsigned int slot = 0) const;
	void Free();
};

