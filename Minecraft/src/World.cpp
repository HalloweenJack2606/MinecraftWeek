#include "World.h"
#include <algorithm>
#include "Input.h"

World::World()
{
	m_Shader.Compile("#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"layout (location = 1) in vec3 uv;\n"
		"out vec3 oUV;\n"
		"uniform mat4 pv;\n"
		"void main()\n"
		"{\n"
		"oUV = uv;\n"
		"gl_Position = pv * vec4(pos.xyz, 1.0);\n"
		"}\n",

		"#version 330 core\n"
		"in vec3 oUV;\n"
		"out vec4 fragColor;\n"
		"uniform sampler2D uTexture;\n"
		"void main()\n"
		"{\n"
		"fragColor = texture(uTexture, vec2(oUV.x, 1.0 - oUV.y)) * vec4(oUV.z, oUV.z, oUV.z, 1.0f);\n"
		"}\n");

	LoadChunk(glm::vec3(0.0f));
	GetChunk(glm::vec3(0.0f))->InitRenderer();
	m_NeighboringChunks = new glm::vec3[8];
}

World::~World()
{
	for (const auto& it : chunks) delete it.second;
	delete[] m_NeighboringChunks;
	m_Shader.Destroy();
}

void World::Update(const glm::vec3& center)
{
	//@hack: chunk reload
	if (Input::GetKeyUp(Key_Q)) for (auto& it : chunks) if(it.second) it.second->SetDirty();
	//std::cout << Time::frameCount << std::endl;
	auto newChunkOffset = PositionToChunk(center);
	this->center = center;
	if (this->centerOffset != newChunkOffset || m_bFirstUpdate)
	{
		m_bFirstUpdate = false;
		glm::vec3 oldCenter = this->center;
		this->center = center;
		this->centerOffset = newChunkOffset;

		std::vector<Chunk*> old;
		for (const auto& it : chunks) old.push_back(it.second);
		chunks.clear();
		
		for (const auto& it : old)
		{
			const auto& itOffset = it->offset;
			if (it == nullptr)
			{
				continue;
			}
			else if (ChunkInBounds(itOffset))
			{
				chunks[glm::vec2(itOffset.x, itOffset.z)] = it;
			}
			else {
				delete it;
			}
		}
		old.clear();
		m_ChunksToLoad = GetEmptyNeighboringChunks();
	}
	ChunkLoading();
	SelectBlock();
}

void World::Render()
{
	Resources::GetTexture("block_atlas").Bind();
	m_Shader.Use();
	m_Shader.SetMat4("pv", m_PVMatrix);
	Frustum::ExtractPlanes(m_PVMatrix);

	std::vector<glm::vec2> positionsNew;
	for (const auto& it : chunks) if (it.second) positionsNew.push_back(it.first);

	std::sort(positionsNew.begin(), positionsNew.end(), [=](const glm::vec2& l, const glm::vec2& r) {
		auto c = PositionToChunk(center);
		auto p = glm::vec2(c.x, c.z);
		return (glm::length(p - l) > glm::length(p - r));
	});

	for (const auto& it : positionsNew)
	{
		if(it != glm::vec2(centerOffset.x, centerOffset.z))
			chunks[it]->Render(m_PVMatrix);
	}
	chunks[glm::vec2(centerOffset.x, centerOffset.z)]->Render(m_PVMatrix);
}

void World::BlockRemove(glm::vec3 position, glm::vec3 direction)
{
	auto chunk = PositionToChunk(position);

	if (auto pChunk = GetChunk(chunk))
	{
		//@note: Set border chunks dirty
		if (pChunk->BlockOnBounds(position))
		{
			pChunk->SetChunkNeighbors();
			for (const auto& neighbor : pChunk->m_LastChunkNeighbors)
			{
				neighbor->SetDirty();
			}
		}

		pChunk->SetData(position, 0);
	}
}

void World::BlockPlace(glm::vec3 position, glm::vec3 direction)
{
	auto chunk = PositionToChunk(position);
		
	if (auto pChunk = GetChunk(chunk))
	{
		if (m_BlockID == 5) pChunk->SetFlag(position + glm::vec3(0.0f, 1.0f, 0.0f), BLOCK_TRANSPARENT);
		pChunk->SetBlockID(position + glm::vec3(0.0f, 1.0f, 0.0f), m_BlockID);
	}
}

glm::vec3 World::PositionToChunk(const glm::vec3 pos) const
{
	glm::vec3 offset = glm::vec3(floor(pos.x / CHUNK_SIZE.x), floor(pos.y / CHUNK_SIZE.y), floor(pos.z / CHUNK_SIZE.z));
	return offset;
}

bool World::ChunkInBounds(glm::vec3 chunkOffset)
{
	return chunkOffset.x < centerOffset.x + renderDistance && chunkOffset.z < centerOffset.z + renderDistance &&
		chunkOffset.x > centerOffset.x - renderDistance && chunkOffset.z > centerOffset.z - renderDistance;
}

bool World::IsChunkLoaded(glm::vec3 offset)
{
	//@note: in Minecraft fashion it doesn't take into account Y chunks, it's chunks are only on a 2D array
	if (GetChunk(offset)) return true;
	return false;
}

void World::LoadChunk(glm::vec3 offset)
{
	chunks[glm::vec2(offset.x, offset.z)] = new Chunk(*this, offset, this->center);
}

std::vector<glm::vec3> World::GetEmptyNeighboringChunks()
{
	std::vector<glm::vec3> v;
	std::vector<glm::vec3> n;

	v.push_back(centerOffset);
	for (short i = 1; i < renderDistance; i++)
	{
		for (const auto& it : v)
		{
			GetNeighboringChunks(it);
			auto nc = m_NeighboringChunks;
			for (int chunk = 0; chunk < 8; chunk++)
			{
				bool doPush = true;
				for (const auto& newChunk : n)
				{
					if (newChunk == nc[chunk])
					{
						doPush = false;
						break;
					}
				}
				if(doPush) n.push_back(nc[chunk]);
			}
		}
		v.clear();
		std::copy(n.begin(), n.end(), std::back_inserter(v));
	}
	n.clear();

	std::copy_if(v.begin(), v.end(), std::back_inserter(n), [=](glm::vec3 lvec)
	{
		return !IsChunkLoaded(lvec);
	});
	return n;
}

void World::GetNeighboringChunks(glm::vec3 offset)
{
	int i = 0;
	m_NeighboringChunks[i++] = glm::vec3(offset.x + 1, 0, offset.z + 0);
	m_NeighboringChunks[i++] = glm::vec3(offset.x - 1, 0, offset.z + 0);
	m_NeighboringChunks[i++] = glm::vec3(offset.x + 0, 0, offset.z + 1);
	m_NeighboringChunks[i++] = glm::vec3(offset.x + 0, 0, offset.z - 1);
	m_NeighboringChunks[i++] = glm::vec3(offset.x + 1, 0, offset.z + 1);
	m_NeighboringChunks[i++] = glm::vec3(offset.x + 1, 0, offset.z - 1);
	m_NeighboringChunks[i++] = glm::vec3(offset.x - 1, 0, offset.z + 1);
	m_NeighboringChunks[i++] = glm::vec3(offset.x - 1, 0, offset.z - 1);
}

void World::ChunkLoading()
{
	if (!m_ChunksToLoad.empty()) {
		auto chunksIt = m_MaxChunkLoadsPerUpdate;
		if (chunksIt > m_ChunksToLoad.size()) chunksIt = m_ChunksToLoad.size();
		for (int i = 0; i < chunksIt; i++)
		{
			LoadChunk(m_ChunksToLoad[i]);
			//m_Futures.push_back(std::async(std::launch::async, [&](const glm::vec3& offset) {
			//	chunks[glm::vec2(offset.x, offset.z)] = new Chunk(*this, offset);
			//}, m_ChunksToLoad[i]));
		}

		//bool chunkLoadingComplete = false;
		//while (!chunkLoadingComplete && !m_Futures.size())
		//{
		//	bool futureCheck = true;
		//	for (const auto& i : m_Futures)
		//	{
		//		auto x = i.wait_for(std::chrono::milliseconds(1));
		//		if (x != std::future_status::ready) { futureCheck = false; break; }
		//	}
		//	chunkLoadingComplete = futureCheck;
		//}
		//m_Futures.clear();

		for (int i = 0; i < chunksIt; i++)
		{
			GetChunk(m_ChunksToLoad[i])->InitRenderer();
			if (auto pChunk = GetChunk(m_ChunksToLoad[i]))
			{
				pChunk->SetChunkNeighbors();
				for (const auto& neighbor : pChunk->m_LastChunkNeighbors)
				{
					if (neighbor) neighbor->SetDirty();
				}
			}
		}
		m_ChunksToLoad.erase(m_ChunksToLoad.begin(), m_ChunksToLoad.begin() + chunksIt);
	}
}

void World::SelectBlock()
{
	if (Input::GetKeyUp(Key_X))
	{
		if (m_BlockID < 9) m_BlockID++;
	}

	if (Input::GetKeyUp(Key_Z))
	{
		if (m_BlockID > 1) m_BlockID--;
	}
}
