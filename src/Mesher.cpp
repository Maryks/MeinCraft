#include "Mesher.h"

Mesher::Mesher()
	: m_chunkletSizeX(WorldConsts::CHUNKLETSIZE_X),
	m_chunkletSizeY(WorldConsts::CHUNKLETSIZE_Y),
	m_chunkletSizeZ(WorldConsts::CHUNKLETSIZE_Z),
	m_chunkSizeX(WorldConsts::CHUNKSIZE_X),
	m_chunkSizeY(WorldConsts::CHUNKSIZE_Y),
	m_chunkSizeZ(WorldConsts::CHUNKSIZE_Z)
{
	m_stride = m_blockData.m_stride;
}

// if a block has an air neighbor
// -- set vertex coordinates of the corresponding face
// -- set texture coordinates
void Mesher::mesh(const std::vector<id_t>& world, coords_t& vertexCoordinates, coords_t& textureCoordinates, xz_t position)
{
	m_worldData = &world;

	m_chunkBlockPositionX = m_chunkSizeX * position.first;
	m_chunkBlockPositionZ = m_chunkSizeZ * position.second;

	m_vertCoords = &vertexCoordinates;
	m_texCoords = &textureCoordinates;

	const float n = 0.5f;
	id_t value;

	for (int x = 0; x < m_chunkSizeX; ++x)
	{
		for (int z = 0; z < m_chunkSizeZ; ++z)
		{
			for (int y = 0; y < m_chunkSizeY; ++y)
			{
				value = this->getBlockID(x, y, z);
				if (value)
				{
					// block right of current block
					if (this->getBlockID(x + 1, y, z) == 0)	// neighbor is air
					{
						this->setVertexCoordinates(
							x + n, y - n, z + n,	// bottom left
							x + n, y - n, z - n,	// bottom right
							x + n, y + n, z - n,	// top right
							x + n, y + n, z + n		// top left
						);
						this->setTextureCoordinates(value, Face::Side);
					}
					// block left
					if (this->getBlockID(x - 1, y, z) == 0)
					{
						this->setVertexCoordinates(
							x - n, y - n, z - n,
							x - n, y - n, z + n,
							x - n, y + n, z + n,
							x - n, y + n, z - n
						);
						this->setTextureCoordinates(value, Face::Side);
					}
					// block above
					if (this->getBlockID(x, y + 1, z) == 0)
					{
						this->setVertexCoordinates(
							x - n, y + n, z + n,
							x + n, y + n, z + n,
							x + n, y + n, z - n,
							x - n, y + n, z - n
						);
						this->setTextureCoordinates(value, Face::Top);
					}
					// block below
					if (this->getBlockID(x, y - 1, z) == 0)
					{
						this->setVertexCoordinates(
							x - n, y - n, z - n,
							x + n, y - n, z - n,
							x + n, y - n, z + n,
							x - n, y - n, z + n
						);
						this->setTextureCoordinates(value, Face::Bottom);
					}
					// block in front
					if (this->getBlockID(x, y, z + 1) == 0)
					{
						this->setVertexCoordinates(
							x - n, y - n, z + n,
							x + n, y - n, z + n,
							x + n, y + n, z + n,
							x - n, y + n, z + n
						);
						this->setTextureCoordinates(value, Face::Side);
					}
					// block behind
					if (this->getBlockID(x, y, z - 1) == 0)
					{
						this->setVertexCoordinates(
							x + n, y - n, z - n,
							x - n, y - n, z - n,
							x - n, y + n, z - n,
							x + n, y + n, z - n
						);
						this->setTextureCoordinates(value, Face::Side);
					}
				}
			}
		}
	}
}

id_t Mesher::getBlockID(int x, int y, int z)
{
	++x;
	++y;
	++z;
	return m_worldData->at(x + (y + z * (m_chunkSizeY + 2)) * (m_chunkSizeX + 2));
}

void Mesher::setTextureCoordinates(id_t value, Face face)
{
	float u;
	float v;
	Block::ID id = static_cast<Block::ID>(value);
	if (face == Face::Top)
	{
		u = m_blockData.m_DataHolder.at(id)->topCoords.u;
		v = m_blockData.m_DataHolder.at(id)->topCoords.v;
	}
	else if (face == Face::Side)
	{
		u = m_blockData.m_DataHolder.at(id)->sideCoords.u;
		v = m_blockData.m_DataHolder.at(id)->sideCoords.v;
	}
	else // if (face == Face::Bottom)
	{
		u = m_blockData.m_DataHolder.at(id)->bottomCoords.u;
		v = m_blockData.m_DataHolder.at(id)->bottomCoords.v;
	}

	u *= m_stride;
	v *= m_stride;

	// bottom left
	m_texCoords->emplace_back(u);
	m_texCoords->emplace_back(v + m_stride);
	// bottom right
	m_texCoords->emplace_back(u + m_stride);
	m_texCoords->emplace_back(v + m_stride);
	// top right
	m_texCoords->emplace_back(u + m_stride);
	m_texCoords->emplace_back(v);
	// top left
	m_texCoords->emplace_back(u);
	m_texCoords->emplace_back(v);
}

void Mesher::setVertexCoordinates(float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float p12)
{
	// bottom left
	m_vertCoords->emplace_back(p1 + m_chunkBlockPositionX);
	m_vertCoords->emplace_back(p2);
	m_vertCoords->emplace_back(p3 + m_chunkBlockPositionZ);
	// bottom right
	m_vertCoords->emplace_back(p4 + m_chunkBlockPositionX);
	m_vertCoords->emplace_back(p5);
	m_vertCoords->emplace_back(p6 + m_chunkBlockPositionZ);
	// top right
	m_vertCoords->emplace_back(p7 + m_chunkBlockPositionX);
	m_vertCoords->emplace_back(p8);
	m_vertCoords->emplace_back(p9 + m_chunkBlockPositionZ);
	// top left
	m_vertCoords->emplace_back(p10 + m_chunkBlockPositionX);
	m_vertCoords->emplace_back(p11);
	m_vertCoords->emplace_back(p12 + m_chunkBlockPositionZ);
}

// static
Block::BlockData  Mesher::m_blockData;