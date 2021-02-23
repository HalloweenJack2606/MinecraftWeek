#include "Chunk.h"

Chunk::Chunk(World& world, glm::vec3 offset, const glm::vec3& playerPos)
{
	this->m_PlayerPosition = &playerPos;
	this->m_World = &world;
	this->offset = offset;
	this->position = offset * CHUNK_SIZE;
	blocks.reserve(CHUNK_VOLUME);
	for (int i = 0; i < CHUNK_VOLUME; i++) blocks.push_back(Block());
	m_NeighborsCoords.reserve(4);
	m_LastChunkNeighbors.reserve(4);
	m_BlockNeighbors = new glm::vec4[6];
	SetNeighborCoords();

	HeightMap heightMap;
	heightMap.reserve(CHUNK_VOLUME);
	heightMap.resize(CHUNK_VOLUME, 0);
	TerrainGenerator::GetHeightMap(this->offset, heightMap, 56.0f);
	TerrainGenerator::GetHeightMap(this->offset, heightMap, 64.0f);
	TerrainGenerator::GetHeightMap(this->offset, heightMap, 72.0f);
	TerrainGenerator::GetSmoothTerrain(*this, heightMap);
}

Chunk::~Chunk()
{
	delete[] m_BlockNeighbors;
	m_VAO.Destroy();
	m_VBO.Destroy();
	m_IBO.Destroy();
}

void Chunk::InitRenderer()
{
	m_VAO.Generate();
	m_VBO.Generate();
	m_IBO.Generate();

	m_VAO.Bind();
	m_VBO.Bind();

	m_VBO.Init();
	m_VAO.Init();

	m_IBO.Bind();
	m_IBO.Init();

	m_IBO.Unbind();
	m_VBO.Unbind();
	m_VAO.Unbind();
}

void Chunk::Render(const glm::mat4& pv)
{
	if (!Frustum::PlaneIntersection(m_World->m_PVMatrix, position, 16)) return;
	m_VAO.Bind();
	m_VBO.Bind();
	m_IBO.Bind();
	if (m_bDirty)
	{
		SetChunkNeighbors();
		m_VertexCount = 0;
		m_IndexCount = 0;
		for (int x = position.x; x < (position.x + CHUNK_SIZE.x); x++)
		{
			for (int y = position.y; y < (position.y + CHUNK_SIZE.y); y++)
			{
				for (int z = position.z; z < (position.z + CHUNK_SIZE.z); z++)
				{
					auto pos = glm::vec3(x, y, z);
					Push(pos, false);
				}
			}
		}
		std::sort(m_TransparentObjects.begin(), m_TransparentObjects.end(), [=](const glm::vec3& l, const glm::vec3& r) {
			auto p = *m_PlayerPosition;
			return glm::length(p - l) > glm::length(p - r);
		});
		for (const auto it : m_TransparentObjects)
		{
			Push(it, true);
		}
	
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_Vertices.size(), &m_Vertices.front());
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * m_Indices.size(), &m_Indices.front());
		m_TransparentObjects.clear();
		m_Vertices.clear();
		m_Indices.clear();
	
		m_bDirty = false;
	}

	glDrawElements(GL_TRIANGLES, m_VertexCount, GL_UNSIGNED_INT, 0);

	m_VAO.Unbind();
	m_VBO.Unbind();
	m_IBO.Unbind();
}

void Chunk::Push(const glm::vec3& cubePos, const bool transparentPass)
{
	if (!GetData(cubePos)) return;
	bool isSolid = !GetFlag(cubePos, BLOCK_TRANSPARENT);
	if (!isSolid && !transparentPass) 
	{
		m_TransparentObjects.push_back(cubePos);
		return;
	}
	int i = 6;
	Direction faces = NONE;
	//@note: border chunk check
	if (BlockOnBounds(cubePos))
	{
		//SetBlockID(cubePos, 9);
		for (const auto& it : m_NeighborsCoords)
		{
			auto tempNeighbor = cubePos + glm::vec3(it);
			auto cpos = m_World->PositionToChunk(tempNeighbor);
			if (auto pChunk = m_World->GetChunk(cpos))
			{
				if (pChunk->GetData(tempNeighbor))
				{
					if (isSolid && (pChunk->GetFlag(tempNeighbor, BLOCK_TRANSPARENT)))
					{
						continue;
					}
					faces |= (Direction)(it).w;
					i--;
				}
			}
		}
	}

	//@note: normal block face checks
	GetNeighbors(cubePos);
	for (int j = 0; j < 6; j++)
	{
		auto it = m_BlockNeighbors[j];
		if (GetData(glm::vec3(it))) {
			if (isSolid && (GetFlag(glm::vec3(it), BLOCK_TRANSPARENT)))
			{
				continue;
			}
			if (!(faces & (Direction)(it).w)) {
				faces |= (Direction)(it).w;
				i--;
			}
		}
	}
	
	if (i <= 0) return;

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, cubePos);

	glm::vec4 vertexPositions[8];
	glm::vec2 uvCoords[4];

	{
		vertexPositions[0] = transform * glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
		vertexPositions[1] = transform * glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f);
		vertexPositions[2] = transform * glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f);
		vertexPositions[3] = transform * glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f);
		vertexPositions[4] = transform * glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f);
		vertexPositions[5] = transform * glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f);
		vertexPositions[6] = transform * glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f);
		vertexPositions[7] = transform * glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f);
	}

	//@opt: this is really slow, don't use std::map | really, I should change it, it's just stupid
	auto id = Block::GetAtlas(GetBlockID(cubePos));

	glm::vec2 uv00 = glm::vec2((id.x * SPRITE_SIZE) / TEXTURE_SIZE, (id.y * SPRITE_SIZE) / TEXTURE_SIZE);
	glm::vec2 uv10 = glm::vec2(((id.x + 1) * SPRITE_SIZE) / TEXTURE_SIZE, (id.y * SPRITE_SIZE) / TEXTURE_SIZE);
	glm::vec2 uv11 = glm::vec2(((id.x + 1) * SPRITE_SIZE) / TEXTURE_SIZE, ((id.y + 1) * SPRITE_SIZE) / TEXTURE_SIZE);
	glm::vec2 uv01 = glm::vec2((id.x * SPRITE_SIZE) / TEXTURE_SIZE, ((id.y + 1) * SPRITE_SIZE) / TEXTURE_SIZE);

	{
		uvCoords[0] = uv00;
		uvCoords[1] = uv10;
		uvCoords[2] = uv11;
		uvCoords[3] = uv01;
	}
	
	if (!(faces & WEST))
	{
		m_Vertices.push_back(vertexPositions[0].x); m_Vertices.push_back(vertexPositions[0].y); m_Vertices.push_back(vertexPositions[0].z); m_Vertices.push_back(uvCoords[0].x); m_Vertices.push_back(uvCoords[0].y); m_Vertices.push_back(0.86f);
		m_Vertices.push_back(vertexPositions[1].x); m_Vertices.push_back(vertexPositions[1].y); m_Vertices.push_back(vertexPositions[1].z); m_Vertices.push_back(uvCoords[1].x); m_Vertices.push_back(uvCoords[1].y); m_Vertices.push_back(0.86f);
		m_Vertices.push_back(vertexPositions[2].x); m_Vertices.push_back(vertexPositions[2].y); m_Vertices.push_back(vertexPositions[2].z); m_Vertices.push_back(uvCoords[2].x); m_Vertices.push_back(uvCoords[2].y); m_Vertices.push_back(0.86f);
		m_Vertices.push_back(vertexPositions[3].x); m_Vertices.push_back(vertexPositions[3].y); m_Vertices.push_back(vertexPositions[3].z); m_Vertices.push_back(uvCoords[3].x); m_Vertices.push_back(uvCoords[3].y); m_Vertices.push_back(0.86f);
		
		m_Indices.push_back(m_IndexCount + 0); m_Indices.push_back(m_IndexCount + 1); m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 3); m_Indices.push_back(m_IndexCount + 0);
		
		m_VertexCount += 6;
		m_IndexCount += 4;
	}
	if (!(faces & EAST))
	{
		m_Vertices.push_back(vertexPositions[7].x); m_Vertices.push_back(vertexPositions[7].y); m_Vertices.push_back(vertexPositions[7].z); m_Vertices.push_back(uvCoords[3].x); m_Vertices.push_back(uvCoords[3].y);  m_Vertices.push_back(0.86f);
		m_Vertices.push_back(vertexPositions[6].x); m_Vertices.push_back(vertexPositions[6].y); m_Vertices.push_back(vertexPositions[6].z); m_Vertices.push_back(uvCoords[2].x); m_Vertices.push_back(uvCoords[2].y);  m_Vertices.push_back(0.86f);
		m_Vertices.push_back(vertexPositions[5].x); m_Vertices.push_back(vertexPositions[5].y); m_Vertices.push_back(vertexPositions[5].z); m_Vertices.push_back(uvCoords[1].x); m_Vertices.push_back(uvCoords[1].y);  m_Vertices.push_back(0.86f);
		m_Vertices.push_back(vertexPositions[4].x); m_Vertices.push_back(vertexPositions[4].y); m_Vertices.push_back(vertexPositions[4].z); m_Vertices.push_back(uvCoords[0].x); m_Vertices.push_back(uvCoords[0].y);  m_Vertices.push_back(0.86f);
		
		m_Indices.push_back(m_IndexCount + 0); m_Indices.push_back(m_IndexCount + 1); m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 3); m_Indices.push_back(m_IndexCount + 0);
		
		m_VertexCount += 6;
		m_IndexCount += 4;
	}
	if (!(faces & SOUTH))
	{
		m_Vertices.push_back(vertexPositions[0].x); m_Vertices.push_back(vertexPositions[0].y); m_Vertices.push_back(vertexPositions[0].z); m_Vertices.push_back(uvCoords[0].x); m_Vertices.push_back(uvCoords[0].y); m_Vertices.push_back(0.80f);
		m_Vertices.push_back(vertexPositions[3].x); m_Vertices.push_back(vertexPositions[3].y); m_Vertices.push_back(vertexPositions[3].z); m_Vertices.push_back(uvCoords[3].x); m_Vertices.push_back(uvCoords[3].y); m_Vertices.push_back(0.80f);
		m_Vertices.push_back(vertexPositions[7].x); m_Vertices.push_back(vertexPositions[7].y); m_Vertices.push_back(vertexPositions[7].z); m_Vertices.push_back(uvCoords[2].x); m_Vertices.push_back(uvCoords[2].y); m_Vertices.push_back(0.80f);
		m_Vertices.push_back(vertexPositions[4].x); m_Vertices.push_back(vertexPositions[4].y); m_Vertices.push_back(vertexPositions[4].z); m_Vertices.push_back(uvCoords[1].x); m_Vertices.push_back(uvCoords[1].y); m_Vertices.push_back(0.80f);
		
		m_Indices.push_back(m_IndexCount + 1); m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 3); m_Indices.push_back(m_IndexCount + 3); m_Indices.push_back(m_IndexCount + 0); m_Indices.push_back(m_IndexCount + 1);
		
		m_VertexCount += 6;
		m_IndexCount += 4;
	}
	if (!(faces & NORTH))
	{
		m_Vertices.push_back(vertexPositions[2].x); m_Vertices.push_back(vertexPositions[2].y); m_Vertices.push_back(vertexPositions[2].z); m_Vertices.push_back(uvCoords[1].x); m_Vertices.push_back(uvCoords[1].y); m_Vertices.push_back(0.80f);
		m_Vertices.push_back(vertexPositions[6].x); m_Vertices.push_back(vertexPositions[6].y); m_Vertices.push_back(vertexPositions[6].z); m_Vertices.push_back(uvCoords[0].x); m_Vertices.push_back(uvCoords[0].y); m_Vertices.push_back(0.80f);
		m_Vertices.push_back(vertexPositions[1].x); m_Vertices.push_back(vertexPositions[1].y); m_Vertices.push_back(vertexPositions[1].z); m_Vertices.push_back(uvCoords[2].x); m_Vertices.push_back(uvCoords[2].y); m_Vertices.push_back(0.80f);
		m_Vertices.push_back(vertexPositions[5].x); m_Vertices.push_back(vertexPositions[5].y); m_Vertices.push_back(vertexPositions[5].z); m_Vertices.push_back(uvCoords[3].x); m_Vertices.push_back(uvCoords[3].y); m_Vertices.push_back(0.80f);
	
		m_Indices.push_back(m_IndexCount + 1); m_Indices.push_back(m_IndexCount + 0); m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 3); m_Indices.push_back(m_IndexCount + 1);
		
		m_VertexCount += 6;
		m_IndexCount += 4;
	}
	if (!(faces & DOWN))
	{
		m_Vertices.push_back(vertexPositions[4].x); m_Vertices.push_back(vertexPositions[4].y); m_Vertices.push_back(vertexPositions[4].z); m_Vertices.push_back(uvCoords[0].x); m_Vertices.push_back(uvCoords[0].y); m_Vertices.push_back(1.0f);
		m_Vertices.push_back(vertexPositions[0].x); m_Vertices.push_back(vertexPositions[0].y); m_Vertices.push_back(vertexPositions[0].z); m_Vertices.push_back(uvCoords[3].x); m_Vertices.push_back(uvCoords[3].y); m_Vertices.push_back(1.0f);
		m_Vertices.push_back(vertexPositions[5].x); m_Vertices.push_back(vertexPositions[5].y); m_Vertices.push_back(vertexPositions[5].z); m_Vertices.push_back(uvCoords[1].x); m_Vertices.push_back(uvCoords[1].y); m_Vertices.push_back(1.0f);
		m_Vertices.push_back(vertexPositions[1].x); m_Vertices.push_back(vertexPositions[1].y); m_Vertices.push_back(vertexPositions[1].z); m_Vertices.push_back(uvCoords[2].x); m_Vertices.push_back(uvCoords[2].y); m_Vertices.push_back(1.0f);
		
		m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 3); m_Indices.push_back(m_IndexCount + 1); m_Indices.push_back(m_IndexCount + 1); m_Indices.push_back(m_IndexCount + 0); m_Indices.push_back(m_IndexCount + 2);
		
		m_VertexCount += 6;
		m_IndexCount += 4;
	}
	if (!(faces & UP))
	{
		m_Vertices.push_back(vertexPositions[2].x); m_Vertices.push_back(vertexPositions[2].y); m_Vertices.push_back(vertexPositions[2].z); m_Vertices.push_back(uvCoords[2].x); m_Vertices.push_back(uvCoords[2].y); m_Vertices.push_back(1.0f);
		m_Vertices.push_back(vertexPositions[3].x); m_Vertices.push_back(vertexPositions[3].y); m_Vertices.push_back(vertexPositions[3].z); m_Vertices.push_back(uvCoords[3].x); m_Vertices.push_back(uvCoords[3].y); m_Vertices.push_back(1.0f);
		m_Vertices.push_back(vertexPositions[6].x); m_Vertices.push_back(vertexPositions[6].y); m_Vertices.push_back(vertexPositions[6].z); m_Vertices.push_back(uvCoords[1].x); m_Vertices.push_back(uvCoords[1].y); m_Vertices.push_back(1.0f);
		m_Vertices.push_back(vertexPositions[7].x); m_Vertices.push_back(vertexPositions[7].y); m_Vertices.push_back(vertexPositions[7].z); m_Vertices.push_back(uvCoords[0].x); m_Vertices.push_back(uvCoords[0].y); m_Vertices.push_back(1.0f);
		
		m_Indices.push_back(m_IndexCount + 1); m_Indices.push_back(m_IndexCount + 0); m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 2); m_Indices.push_back(m_IndexCount + 3); m_Indices.push_back(m_IndexCount + 1);
		
		m_VertexCount += 6;
		m_IndexCount += 4;
	}
}

void Chunk::SetFlag(glm::vec3 pos, const BlockFlags&& flag)
{
	if (BlockInBounds(pos))
	{
		SetDirty();
		PositionToBlock(pos - position).flags |= flag;
	}
}

bool Chunk::GetFlag(glm::vec3 pos, const BlockFlags&& flag)
{
	if (BlockInBounds(pos))
	{
		return (PositionToBlock(pos - position).flags & flag);
	}
	return false;
}

void Chunk::SetData(glm::vec3 pos, std::uint32_t data)
{
	if (BlockInBounds(pos))
	{
		SetDirty();
		PositionToBlock((pos - position)).id = data;
	}
}

std::uint32_t Chunk::GetData(glm::vec3 pos)
{
	if(BlockInBounds(pos)) return PositionToBlock((pos - position)).id;
	return 0;
}
  
void Chunk::SetBlockID(glm::vec3 pos, std::uint16_t id)
{
	if (BlockInBounds(pos))
	{
		SetDirty();
		PositionToBlock((pos - position)).id = id;
	}
}

std::uint16_t Chunk::GetBlockID(glm::vec3 pos)
{
	if (BlockInBounds(pos)) return PositionToBlock((pos - position)).id;
	return 0;
}

bool Chunk::BlockInBounds(glm::vec3 pos)
{
	pos -= position;
	return pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
		pos.x < CHUNK_SIZE.x && pos.y < CHUNK_SIZE.y && pos.z < CHUNK_SIZE.z;
}

bool Chunk::BlockOnBounds(glm::vec3 pos)
{
	//@note: Y check disabled since chunks don't stack on the Y axis
	pos -= position;
	return (pos.x == 0 || /*pos.y == 0 || */pos.z == 0 ||
		pos.x == (CHUNK_SIZE.x - 1) || /*pos.y == (CHUNK_SIZE.y - 1) || */pos.z == (CHUNK_SIZE.z - 1));
}

void Chunk::GetNeighbors(const glm::vec3& pos)
{
	int i = 0;
	m_BlockNeighbors[i++] = glm::vec4(pos, 0.0f) + glm::vec4( 1.0f, 0.0f, 0.0f, NORTH);
	m_BlockNeighbors[i++] = glm::vec4(pos, 0.0f) + glm::vec4(-1.0f, 0.0f, 0.0f, SOUTH);
	m_BlockNeighbors[i++] = glm::vec4(pos, 0.0f) + glm::vec4( 0.0f, 1.0f, 0.0f, UP);
	m_BlockNeighbors[i++] = glm::vec4(pos, 0.0f) + glm::vec4( 0.0f,-1.0f, 0.0f, DOWN);
	m_BlockNeighbors[i++] = glm::vec4(pos, 0.0f) + glm::vec4( 0.0f, 0.0f, 1.0f, EAST);
	m_BlockNeighbors[i++] = glm::vec4(pos, 0.0f) + glm::vec4( 0.0f, 0.0f,-1.0f, WEST);
}

Block& Chunk::PositionToBlock(glm::vec3 p)
{
	return blocks[p.x + CHUNK_SIZE.x * (p.y + CHUNK_SIZE.y * p.z)];
}

void Chunk::SetChunkNeighbors()
{
	m_LastChunkNeighbors.clear();
	m_LastChunkNeighbors.push_back(m_World->GetChunk(offset + glm::vec3(1.0f, 0.0f, 0.0f)));
	m_LastChunkNeighbors.push_back(m_World->GetChunk(offset + glm::vec3(-1.0f, 0.0f, 0.0f)));
	m_LastChunkNeighbors.push_back(m_World->GetChunk(offset + glm::vec3(0.0f, 0.0f, 1.0f)));
	m_LastChunkNeighbors.push_back(m_World->GetChunk(offset + glm::vec3(0.0f, 0.0f, -1.0f)));
}

void Chunk::SetNeighborCoords()
{
	m_NeighborsCoords.push_back(glm::vec4( 1.0f,  0.0f,  0.0f, NORTH));
	m_NeighborsCoords.push_back(glm::vec4(-1.0f,  0.0f,  0.0f, SOUTH));
	m_NeighborsCoords.push_back(glm::vec4( 0.0f,  0.0f,  1.0f, EAST));
	m_NeighborsCoords.push_back(glm::vec4( 0.0f,  0.0f, -1.0f, WEST));
}
