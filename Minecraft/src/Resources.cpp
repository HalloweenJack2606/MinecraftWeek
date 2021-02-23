#include "Resources.h"

std::map<std::string, Texture> Resources::s_Textures;

void Resources::Free()
{
    for (auto& it : s_Textures) it.second.Free();
    s_Textures.clear();
}

Texture Resources::LoadTexture(const char* path, std::string name)
{
    Texture tex;

    int width, height;
    BYTE* data;

    data = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
    tex.GenerateTexture(width, height, data);
    SOIL_free_image_data(data);
    s_Textures[name] = tex;
    return tex;
}

Texture Resources::GetTexture(std::string name)
{
    return s_Textures[name];
}
