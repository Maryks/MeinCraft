#ifndef MESHER_H
#define MESHER_H

#include <atomic>
#include <cassert>
#include <vector>

#include "BlockData.h"
#include "typedefs.h"
#include "World.h"

enum Face
{
	Top,
	Side,
	Bottom
};

class Mesher
{
public:
	Mesher();

	void mesh(const std::vector<id_t>&, coords_t&, coords_t&, xz_t);

private:
	id_t getBlockID(int, int, int);
	void setTextureCoordinates(id_t, Face);
	void setVertexCoordinates(float, float, float, float, float, float, float, float, float, float, float, float);

	const int m_chunkletSizeX;
	const int m_chunkletSizeY;
	const int m_chunkletSizeZ;
	const int m_chunkSizeX;
	const int m_chunkSizeY;
	const int m_chunkSizeZ;
	int m_chunkBlockPositionX;
	int m_chunkBlockPositionZ;
	float m_stride;
		
	coords_t* m_vertCoords;
	coords_t* m_texCoords;
	const std::vector<id_t>* m_worldData;

	static Block::BlockData m_blockData;	// doesn't have to be static?
};

#endif // !MESHER_H
