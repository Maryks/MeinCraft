#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <map>
#include <mutex>
#include <thread>

#include "typedefs.h"
#include "World.h"

class Camera;
class Chunk;
class Renderer;

class WorldManager
{
public:
	WorldManager() = delete;
	WorldManager(Camera&, Renderer&);
	~WorldManager();

	void addToRenderer(Renderer&);
	void generateWorld(Camera&, Renderer&);
	void initializeChunkMap();

	xz_t m_centerChunk;
	xz_t m_lastCenterChunk;
	std::map<xz_t, Chunk> m_chunks;
	bool m_worldExists;
	const int m_worldSize;
	const int m_worldSizeHalf;

private:
	xz_t getArrayIndex(int, int);
	void loadChunks();
	void markForDestruction();

	World m_world;
	std::mutex m_threadMutex;
	std::vector<std::thread> m_thread;
};

#endif // !WORLDMANAGER_H
