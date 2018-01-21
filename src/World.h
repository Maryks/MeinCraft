#ifndef WORLD_H
#define WORLD_H

#include <SFML\Graphics.hpp>

#include <iostream>
#include <vector>

#include "BlockData.h"
#include "typedefs.h"

namespace WorldConsts
{
	// chunklet size in blocks
	const int CHUNKLETSIZE_X = 16;
	const int CHUNKLETSIZE_Y = 16;
	const int CHUNKLETSIZE_Z = 16;
	// chunk size in blocks
	const int CHUNKSIZE_X = CHUNKLETSIZE_X * 1;
	const int CHUNKSIZE_Y = CHUNKLETSIZE_Y * 4;
	const int CHUNKSIZE_Z = CHUNKLETSIZE_Z * 1;
	// generate (world size * world size) chunks around the player (must be odd number!)
	const int WORLDSIZE = 3;
}

class World
{
public:
	World();

	std::vector<id_t> getChunk(int, int);

private:
	void generateChunk(int, int);
	int getIndex(int, int, int);

	int m_chunkSizeX;
	int m_chunkSizeY;
	int m_chunkSizeZ;
	int m_nofBlocks;

	std::vector<id_t> m_chunk;
};

#endif // !WORLD_H
