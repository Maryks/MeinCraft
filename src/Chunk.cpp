#include "Camera.h"
#include "Renderer.h"
#include "World.h"

#include "Chunk.h"

Chunk::Chunk()
	:m_hasMesh(false),
	m_hasWorldData(false),
	m_isMarkedForDestruction(true)
{}

void Chunk::addVaoToRenderer(Renderer& renderer)
{
	this->makeVao();
	renderer.addVao(m_vao, m_vboVert, m_vboTex, m_vertCoords.size() / 3);
}

void Chunk::addWorldData(World& world, xz_t position)
{
	m_vertCoords.clear();
	m_texCoords.clear();
	m_worldData = world.getChunk(position.first, position.second);
	m_hasWorldData = true;
	m_position = position;
}

void Chunk::makeMesh()
{
	m_mesher.mesh(m_worldData, m_vertCoords, m_texCoords, m_position);
	m_hasMesh = true;
}

void Chunk::makeVao()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vboVert);
	glGenBuffers(1, &m_vboTex);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboVert);
	glBufferData(GL_ARRAY_BUFFER, m_vertCoords.size() * sizeof(float), &m_vertCoords.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboTex);
	glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(float), &m_texCoords.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
