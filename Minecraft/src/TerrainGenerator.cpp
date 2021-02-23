#include "TerrainGenerator.h"

HeightMap TerrainGenerator::GetHeightMap(const glm::vec3& chunkOffset, HeightMap& heightMap, const float amplifier)
{
	for (int z = 0; z < CHUNK_SIZE.z; z++)
	{
		for (int x = 0; x < CHUNK_SIZE.x; x++)
		{
			int blockX = x + (chunkOffset.x * CHUNK_SIZE.x);
			int blockZ = z + (chunkOffset.z * CHUNK_SIZE.z);

			float value = glm::simplex(glm::vec2(blockX, blockZ) / amplifier);

			value = (value + 1.0f) / 2.0f;

			value *= 8.0f;

			heightMap[z* CHUNK_SIZE.x + x] += static_cast<std::uint8_t>(value);
		}
	}

	return heightMap;
}

void TerrainGenerator::GetSmoothTerrain(Chunk& chunk, HeightMap& heightmap)
{
	for (int z = 0; z < CHUNK_SIZE.z; z++)
	{
		for (int x = 0; x < CHUNK_SIZE.x; x++)
		{
			auto height = GROUND_LEVEL + heightmap[z * CHUNK_SIZE.x + x];
			for (int y = 0; y < CHUNK_SIZE.y; y++)
			{
				glm::vec3 blockArrayPosition = glm::vec3(x, y, z) + chunk.position;
				if (height <= WATER_LEVEL)
				{
					if (y == 0)
					{
						chunk.SetData(blockArrayPosition, 1);
						chunk.SetBlockID(blockArrayPosition, 9);
					}
					else if (y <= height)
					{
						chunk.SetData(blockArrayPosition, 1);
						chunk.SetBlockID(blockArrayPosition, 4);
					}
					else if(y <= WATER_LEVEL && y > height)
					{
						chunk.SetData(blockArrayPosition, 1);
						chunk.SetBlockID(blockArrayPosition, 10);
						chunk.SetFlag(blockArrayPosition, BLOCK_TRANSPARENT);
					}
					continue;
				}
				if (y == 0)
				{
					chunk.SetData(blockArrayPosition, 1);
					chunk.SetBlockID(blockArrayPosition, 9);
				}
				else if (y == height)
				{
					chunk.SetData(blockArrayPosition, 1);
					chunk.SetBlockID(blockArrayPosition, 8);
					if (rand() % 250 + 1 == 5) MakeTree(chunk, blockArrayPosition);
				}
				else if (y == height - 1)
				{
					chunk.SetData(blockArrayPosition, 1);
					chunk.SetBlockID(blockArrayPosition, 2);
				}
				else if (y <= height)
				{
					chunk.SetData(blockArrayPosition, 1);
					chunk.SetBlockID(blockArrayPosition, 1);
				}
			}
		}
	}
}

void TerrainGenerator::MakeTree(Chunk& chunk, const glm::vec3& position)
{
	//@note: wood
	glm::vec3 wood = glm::vec3(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 5; i++)
	{
		chunk.SetData(position + wood, 1);
		chunk.SetBlockID(position + wood, 3);
		wood.y++;
	}

	//@note: leaves
	chunk.SetData(position + wood, 1);
	chunk.SetBlockID(position + wood, 6);
	chunk.SetFlag(position + wood, BLOCK_TRANSPARENT);
	glm::vec3* leaves = new glm::vec3[32];
	leaves[0] = glm::vec3(1.0f, wood.y -1.0f, 0.0f);
	leaves[1] = glm::vec3(1.0f, wood.y - 1.0f, 1.0f);
	leaves[2] = glm::vec3(1.0f, wood.y - 1.0f, -1.0f);
	leaves[3] = glm::vec3(0.0f, wood.y - 1.0f, 1.0f);
	leaves[4] = glm::vec3(0.0f, wood.y - 1.0f, -1.0f);
	leaves[5] = glm::vec3(-1.0f, wood.y - 1.0f, 1.0f);
	leaves[6] = glm::vec3(-1.0f, wood.y - 1.0f, 0.0f);
	leaves[7] = glm::vec3(-1.0f, wood.y - 1.0f, -1.0f);

	leaves[8] = glm::vec3( 1.0f, wood.y - 2.0f, 0.0f);
	leaves[9] = glm::vec3( 1.0f, wood.y - 2.0f, 1.0f);
	leaves[10] = glm::vec3( 1.0f, wood.y - 2.0f, -1.0f);
	leaves[11] = glm::vec3( 0.0f, wood.y - 2.0f, 1.0f);
	leaves[12] = glm::vec3( 0.0f, wood.y - 2.0f, -1.0f);
	leaves[13] = glm::vec3(-1.0f, wood.y - 2.0f, 1.0f);
	leaves[14] = glm::vec3(-1.0f, wood.y - 2.0f, 0.0f);
	leaves[15] = glm::vec3(-1.0f, wood.y - 2.0f, -1.0f);

	leaves[16] =  glm::vec3( 2.0f, wood.y - 2.0f,  0.0f);
	leaves[17] =  glm::vec3( 2.0f, wood.y - 2.0f,  2.0f);
	leaves[18] = glm::vec3( 2.0f, wood.y - 2.0f, -2.0f);
	leaves[19] = glm::vec3( 0.0f, wood.y - 2.0f,  2.0f);
	leaves[20] = glm::vec3( 0.0f, wood.y - 2.0f, -2.0f);
	leaves[21] = glm::vec3(-2.0f, wood.y - 2.0f,  2.0f);
	leaves[22] = glm::vec3(-2.0f, wood.y - 2.0f,  0.0f);
	leaves[23] = glm::vec3(-2.0f, wood.y - 2.0f, -2.0f);

	leaves[24] = glm::vec3( 2.0f, wood.y - 2.0f,  1.0f);
	leaves[25] = glm::vec3( 2.0f, wood.y - 2.0f, -1.0f);
	leaves[26] = glm::vec3(-2.0f, wood.y - 2.0f,  1.0f);
	leaves[27] = glm::vec3(-2.0f, wood.y - 2.0f, -1.0f);
	leaves[28] = glm::vec3( 1.0f, wood.y - 2.0f,  2.0f);
	leaves[29] = glm::vec3( 1.0f, wood.y - 2.0f, -2.0f);
	leaves[30] = glm::vec3(-1.0f, wood.y - 2.0f,  2.0f);
	leaves[31] = glm::vec3(-1.0f, wood.y - 2.0f, -2.0f);
	for (int i = 0; i < 32; i++)
	{
		if (chunk.BlockInBounds(position + leaves[i]))
		{
			chunk.SetData(position + leaves[i], 1);
			chunk.SetBlockID(position + leaves[i], 6);
			chunk.SetFlag(position + leaves[i], BLOCK_TRANSPARENT);
		}
		else
		{

		}
	}
	delete[] leaves;
}
