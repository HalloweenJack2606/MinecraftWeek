#include "Block.h"

BlockTextureMap Block::blockAtlas;
bool Block::s_bIDSet = false;

void Block::SetIDs()
{
	blockAtlas[0] =  glm::vec2(0, 15);	//@block: andesite
	blockAtlas[1] =  glm::vec2(1, 15);	//@block: stone
	blockAtlas[2] =  glm::vec2(2, 15);	//@block: dirt
	blockAtlas[3] =  glm::vec2(4, 14);	//@block: wood side
	blockAtlas[4] =  glm::vec2(2, 14);	//@block: sand
	blockAtlas[5] =  glm::vec2(1, 12);	//@block: glass
	blockAtlas[6] =  glm::vec2(4,  7);	//@block: leaves
	blockAtlas[7] =  glm::vec2(4, 15);	//@block: sand
	blockAtlas[8] =  glm::vec2(8, 11);	//@block: grass
	blockAtlas[9] =  glm::vec2(1, 14);	//@block: bedrock
	blockAtlas[10] = glm::vec2(15, 2);	//@block: water
	s_bIDSet = true;
}

glm::vec2 Block::GetAtlas(std::uint16_t id)
{
	if (!s_bIDSet) SetIDs();
	return blockAtlas[id];
}