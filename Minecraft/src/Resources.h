#pragma once
#include <map>
#include <iostream>
#include <string>
#include <src/SOIL.h>

#include "Texture.h"
#include "Shader.h"

class Resources
{
private:
	static std::map<std::string, Texture> s_Textures;
public:
	static void Free();
	static Texture LoadTexture(const char* path, std::string name);
	static Texture GetTexture(std::string name);
};

