#include "World.h"

World::World()
{
	m_chunkSizeX = WorldConsts::CHUNKSIZE_X;
	m_chunkSizeY = WorldConsts::CHUNKSIZE_Y;
	m_chunkSizeZ = WorldConsts::CHUNKSIZE_Z;
}

std::vector<id_t> World::getChunk(int chunkPositionX, int chunkPositionZ)
{
	this->generateChunk(chunkPositionX, chunkPositionZ);

	return m_chunk;
}

void World::generateChunk(int chunkPositionX, int chunkPositionZ)
{
	// initialize chunk with air
	for (int i = 0; i < (m_chunkSizeX+2)*(m_chunkSizeY+2)*(m_chunkSizeZ+2); ++i)
	{
		m_chunk.emplace_back(Block::ID::Air);
	}

	for (int x = -1; x <= m_chunkSizeX; ++x)
	{
		for (int z = -1; z <= m_chunkSizeZ; ++z)
		{
			for (int y = -1; y <= m_chunkSizeY; ++y)
			{
				// fill first 10 layers with stone
				// second 10 layers with dirt
				// and third 10 layers with grass
				if (30 > y && y >= 20)
					m_chunk[this->getIndex(x, y, z)] = Block::ID::Grass;
				if (20 > y && y >= 10)
					m_chunk[this->getIndex(x, y, z)] = Block::ID::Dirt;
				if (10 > y && y >= 0)
					m_chunk[this->getIndex(x, y, z)] = Block::ID::Stone;
			}
		}
	}
}

int World::getIndex(int x, int y, int z)
{
	++x;
	++y;
	++z;
	return (x + (y + z * (m_chunkSizeY + 2)) * (m_chunkSizeX + 2));
}