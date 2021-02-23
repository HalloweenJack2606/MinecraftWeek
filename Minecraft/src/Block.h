#pragma once
#include <map>
#include <cstdint>
#include <glm/glm.hpp>

typedef std::map<std::uint16_t, glm::vec2> BlockTextureMap;

enum BlockFlags : std::uint16_t {
    BLOCK_NONE = 0,
    BLOCK_TRANSPARENT = 1,
    BLOCK_TEST_FLAG_X = 2,
    BLOCK_TEST_FLAG_Y = 4,
    BLOCK_TEST_FLAG_Z = 8
};

struct Block
{
    static BlockTextureMap blockAtlas;
    static bool s_bIDSet;
    BlockFlags flags = BLOCK_NONE;
    std::uint16_t id = 0;
    static void SetIDs();
    static glm::vec2 GetAtlas(std::uint16_t);
};

inline BlockFlags operator|(BlockFlags a, BlockFlags b)
{
    return static_cast<BlockFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline BlockFlags& operator|=(BlockFlags& a, BlockFlags b)
{
    a = a | b;
    return a;
}