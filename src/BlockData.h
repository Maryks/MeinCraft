#ifndef BLOCKDATA_H
#define BLOCKDATA_H

#include <iostream>
#include <map>
#include <memory>

#include "typedefs.h"

namespace Block
{
	enum ID : id_t
	{
		Air,
		Grass,
		Stone,
		Dirt,
		nofIDs
	};

	struct uvCoordinates
	{
		float u;
		float v;
	};

	struct Data
	{
		ID id;
		uvCoordinates topCoords;
		uvCoordinates sideCoords;
		uvCoordinates bottomCoords;
	};

	class BlockData
	{
	public:
		BlockData();

		std::map<Block::ID, std::unique_ptr<Data>> m_DataHolder;

		float m_stride;	// offset between adjacent textures in texture sheet
	};
}

#endif // !BLOCKDATA_H
