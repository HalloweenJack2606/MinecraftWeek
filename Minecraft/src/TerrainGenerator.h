#pragma once
#define GROUND_LEVEL 64
#define WATER_LEVEL GROUND_LEVEL + 8

#include <array>
#include <glm/gtc/noise.hpp>
#include "Chunk.h"

typedef std::vector<std::uint8_t> HeightMap;

class Chunk;

class TerrainGenerator
{
public:
	static HeightMap GetHeightMap(const glm::vec3& chunkOffset, HeightMap& heightmap, const float amplifier = 64.0f);
	static void GetSmoothTerrain(Chunk& chunk, HeightMap& heightmap);
private:
	static void MakeTree(Chunk& chunk, const glm::vec3& position);
};

