#ifndef CHUNK_H
#define CHUNK_H

#include <GL\glew.h>

#include <iostream>
#include <vector>

#include "Mesher.h"
#include "typedefs.h"

class Camera;
class Renderer;
class World;

class Chunk
{
public:
	Chunk();

	void addVaoToRenderer(Renderer&);
	void addWorldData(World&, xz_t);
	void makeMesh();

	bool m_hasMesh;
	bool m_hasWorldData;
	bool m_isMarkedForDestruction;
	xz_t m_position;

private:
	void makeVao();

	GLuint m_vao;
	GLuint m_vboVert;
	GLuint m_vboTex;

	Mesher m_mesher;

	coords_t m_vertData;
	coords_t m_texData;
	coords_t m_vertCoords;
	coords_t m_texCoords;
	std::vector<id_t> m_worldData;
};

#endif // !CHUNK_H
