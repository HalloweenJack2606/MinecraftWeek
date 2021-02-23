#pragma once
#include "Time.h"
#include "Chunk.h"
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <mutex>
#include <future>

class Chunk;

typedef std::unordered_map<glm::vec2, Chunk*> ChunkMap;

class World
{
public:
	short renderDistance = 4;
	const glm::vec3 worldZero = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 center = worldZero;
	glm::vec3 centerOffset = worldZero;
	ChunkMap chunks;
private:
	friend class Chunk;

	int m_BlockID = 1;

	const short m_MaxChunkLoadsPerUpdate = 1;
	bool m_bFirstUpdate = true;

	Shader m_Shader = Shader();
	glm::mat4 m_PVMatrix = glm::mat4(0.0f);
	glm::vec3* m_NeighboringChunks = nullptr;

	std::vector<glm::vec3> m_ChunksToLoad;
	std::vector<std::future<void>> m_Futures;
public:
	World();
	~World();
	void Update(const glm::vec3& center);
	void Render();

	void BlockRemove(glm::vec3 position, glm::vec3 direction);
	void BlockPlace(glm::vec3 position, glm::vec3 direction);
	inline void SetPV(const glm::mat4 mat) { m_PVMatrix = mat; }
	inline Chunk* GetChunk(glm::vec3 offset) { return chunks[glm::vec2(offset.x, offset.z)]; }
	glm::vec3 PositionToChunk(const glm::vec3 pos) const;
private:
	bool ChunkInBounds(glm::vec3 offset);
	bool IsChunkLoaded(glm::vec3 offset);
	void LoadChunk(glm::vec3 offset);
	std::vector<glm::vec3> GetEmptyNeighboringChunks();
	void GetNeighboringChunks(glm::vec3 offset);
	void ChunkLoading();

	void SelectBlock();
};

