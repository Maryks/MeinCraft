#include "World.h"

World::World()
{
	m_chunkSizeX = WorldConsts::CHUNKSIZE_X;
	m_chunkSizeY = WorldConsts::CHUNKSIZE_Y;
	m_chunkSizeZ = WorldConsts::CHUNKSIZE_Z;
	m_nofBlocks = m_chunkSizeX * m_chunkSizeY * m_chunkSizeZ;
}

std::vector<id_t> World::getChunk(int chunkPositionX, int chunkPositionZ)
{
	this->generateChunk(chunkPositionX, chunkPositionZ);

	return m_chunk;
}

void World::generateChunk(int chunkPositionX, int chunkPositionZ)
{
	// initialize chunk with air
	m_chunk.clear();
	for (std::vector<id_t>::size_type i = 0; i < static_cast<std::vector<id_t>::size_type>(m_nofBlocks); ++i)
	{
		m_chunk.emplace_back(Block::ID::Air);
	}

	for (int x = 0; x < m_chunkSizeX; ++x)
	{
		for (int z = 0; z < m_chunkSizeZ; ++z)
		{
			for (int y = 0; y < m_chunkSizeY; ++y)
			{
				// fill first 10 layers with stone
				// second 10 layers with dirt
				// and third 10 layers with grass
				if (30 > y && y >= 20)
					m_chunk.at(this->getIndex(x, y, z)) = Block::ID::Grass;
				if (20 > y && y >= 10)
					m_chunk.at(this->getIndex(x, y, z)) = Block::ID::Dirt;
				if (10 > y && y >= 0)
					m_chunk.at(this->getIndex(x, y, z)) = Block::ID::Stone;
			}
		}
	}
}

int World::getIndex(int x, int y, int z)
{
	return (x + (y + z * m_chunkSizeY) * m_chunkSizeX);
}