#pragma once
#define CHUNK_SIZE (glm::vec3 {16.0f, 256.0f, 16.0f})
#define CHUNK_VOLUME (CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z)
#define TEXTURE_SIZE 256.0f
#define SPRITE_SIZE 16.0f

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Resources.h"
#include "Block.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "World.h"
#include "TerrainGenerator.h"
#include "Frustum.h"

class TerrainGenerator;
class World;

enum Direction {
    NONE  = 0,
    NORTH = 1,
    SOUTH = 2,
    EAST  = 4,
    WEST  = 8,
    UP    = 16,
    DOWN  = 32
};

class Chunk
{
public:
	glm::vec3 offset; //@note: chunk number
	glm::vec3 position; //@note: chunk position
    std::vector<Block> blocks;
private:
    friend class World;
    std::size_t m_VertexCount = 0;
    std::size_t m_IndexCount = 0;
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;

    VertexArray m_VAO;
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;

    World* m_World = nullptr;
    const glm::vec3* m_PlayerPosition = nullptr;

    bool m_bDirty = true;
    glm::vec4* m_BlockNeighbors;

    std::vector<Chunk*> m_LastChunkNeighbors;
    std::vector<glm::vec4> m_NeighborsCoords;
    std::vector<glm::vec3> m_TransparentObjects;
public:
    Chunk(World& world, glm::vec3 offset, const glm::vec3& playerPos);
    ~Chunk();
    void InitRenderer();
	void Render(const glm::mat4& pv);
    void SetFlag(glm::vec3 pos, const BlockFlags&& flag);
    bool GetFlag(glm::vec3 pos, const BlockFlags&& flag);
    void SetData(glm::vec3 pos, std::uint32_t data);
    std::uint32_t GetData(glm::vec3 pos);
    void SetBlockID(glm::vec3 pos, std::uint16_t id);
    std::uint16_t GetBlockID(glm::vec3 pos);
    void SetChunkNeighbors();
    inline void SetDirty() { m_bDirty = true; }
    bool BlockInBounds(glm::vec3 pos);
private:
    void Push(const glm::vec3& cubePos, const bool transparentPass = false);

    bool BlockOnBounds(glm::vec3 pos);

    void GetNeighbors(const glm::vec3& pos);
    Block& PositionToBlock(glm::vec3 p);

    void SetNeighborCoords();
};

inline Direction operator|(Direction a, Direction b)
{
    return static_cast<Direction>(static_cast<int>(a) | static_cast<int>(b));
}

inline Direction& operator|=(Direction& a, Direction b)
{
    a = a | b;
    return a;
}