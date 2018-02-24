#include "World.h"

World::World()
{
	m_chunkSizeX = WorldConsts::CHUNKSIZE_X;
	m_chunkSizeY = WorldConsts::CHUNKSIZE_Y;
	m_chunkSizeZ = WorldConsts::CHUNKSIZE_Z;
}

std::vector<id_t> World::getChunk(int chunkPositionX, int chunkPositionZ)
{
	m_chunk.clear();
	m_chunk.shrink_to_fit();

	this->generateChunk(chunkPositionX, chunkPositionZ);

	return m_chunk;
}

void World::generateChunk(int chunkPositionX, int chunkPositionZ)
{
	// initialize chunk with air
	m_chunk.assign((m_chunkSizeX + 2)*(m_chunkSizeY + 2)*(m_chunkSizeZ + 2), Block::ID::Air);

	for (int x = 0; x < m_chunkSizeX; ++x)
	{
		for (int z = 0; z < m_chunkSizeZ; ++z)
		{
			for (int y = -1; y <= m_chunkSizeY; ++y)
			{
				if (chunkPositionX % 2 != 0 && chunkPositionZ % 2 != 0)
				{
					if (y == 11)
					{
						m_chunk[this->getIndex(x, y, z)] = Block::ID::Dirt;
					}
					if (y == 12)
					{
						m_chunk[this->getIndex(x, y, z)] = Block::ID::Grass;
					}
				}
				else
				{
					if (y == 10)
					{
						m_chunk[this->getIndex(x, y, z)] = Block::ID::Stone;
					}
				}
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