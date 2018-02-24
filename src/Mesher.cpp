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
	m_chunkSizeXp2 = m_chunkSizeX + 2;
	m_chunkSizeYp2 = m_chunkSizeY + 2;
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
	return m_worldData->at(x + (y + z * (m_chunkSizeYp2)) * (m_chunkSizeXp2));
}

void Mesher::setTextureCoordinates(id_t value, Face face)
{
	float u;
	float v;
	Block::ID id = static_cast<Block::ID>(value);
	Block::uvCoordinates uv;
	if (face == Face::Top)
	{
		uv = m_blockData.m_DataHolder[id]->topCoords;
		u = uv.u;
		v = uv.v;
	}
	else if (face == Face::Side)
	{
		uv = m_blockData.m_DataHolder[id]->sideCoords;
		u = uv.u;
		v = uv.v;
	}
	else // if (face == Face::Bottom)
	{
		uv = m_blockData.m_DataHolder[id]->bottomCoords;
		u = uv.u;
		v = uv.v;
	}

	u *= m_stride;
	v *= m_stride;

	m_texCoords->insert(m_texCoords->end(), {
		// bottom left
		u,
		v + m_stride,
		// bottom right
		u + m_stride,
		v + m_stride,
		// top right
		u + m_stride,
		v,
		// top left
		u,
		v});
}

void Mesher::setVertexCoordinates(float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float p12)
{
	m_vertCoords->insert(m_vertCoords->end(), {
		// bottom left
		p1 + m_chunkBlockPositionX,
		p2,
		p3 + m_chunkBlockPositionZ,
		// bottom right
		p4 + m_chunkBlockPositionX,
		p5,
		p6 + m_chunkBlockPositionZ,
		// top right
		p7 + m_chunkBlockPositionX,
		p8,
		p9 + m_chunkBlockPositionZ,
		// top left
		p10 + m_chunkBlockPositionX,
		p11,
		p12 + m_chunkBlockPositionZ});
}

// static
Block::BlockData  Mesher::m_blockData;