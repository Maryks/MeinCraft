#include "World.h"

World::World()
{
	m_chunkSizeX = WorldConsts::CHUNKSIZE_X;
	m_chunkSizeY = WorldConsts::CHUNKSIZE_Y;
	m_chunkSizeZ = WorldConsts::CHUNKSIZE_Z;
	m_nofBlocks = m_chunkSizeX * m_chunkSizeY * m_chunkSizeZ;
}

void World::generateChunk(int chunkPositionX, int chunkPositionZ)
{

}

/*int World::getIndex(int x, int y, int z)
{
	
}*/
