#include <chrono>

#include "Camera.h"
#include "Chunk.h"
#include "Renderer.h"

#include "WorldManager.h"

WorldManager::WorldManager(Camera& camera, Renderer& renderer)
	: m_worldExists(true),
	m_worldSize(WorldConsts::WORLDSIZE),
	m_worldSizeHalf(WorldConsts::WORLDSIZE / 2)
{
	int xCamera = static_cast<int>(std::floor(camera.getPosition().x));
	int zCamera = static_cast<int>(std::floor(camera.getPosition().z));
	int xCurrentCenterChunk = xCamera >= 0 ? xCamera / WorldConsts::CHUNKSIZE_X : xCamera / WorldConsts::CHUNKSIZE_X - 1;
	int zCurrentCenterChunk = zCamera >= 0 ? zCamera / WorldConsts::CHUNKSIZE_Z : zCamera / WorldConsts::CHUNKSIZE_Z - 1;
	m_lastCenterChunk = xz_t(xCurrentCenterChunk, zCurrentCenterChunk);

	this->initializeChunkMap();

	m_thread.emplace_back([&]() {
		while (m_worldExists)
		{
			this->generateWorld(camera, renderer);
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	});
	std::cout << "THREAD::World generation thread launched." << std::endl;
}

// join world generation thread
WorldManager::~WorldManager()
{
	m_worldExists = false;
	for (auto& t : m_thread)
	{
		t.join();
	}
	std::cout << "THREAD::World generation thread joined. (" << m_thread.size() << ")" << std::endl;
	if (m_chunks.size() != m_worldSize * m_worldSize)
	{
		std::cout << "Number of chunks in m_chunks: " << m_chunks.size() << std::endl;
		std::cout << "There should be WORLDSIZE times WORLDSIZE chunks." << std::endl;
	}
}

void WorldManager::addToRenderer(Renderer& renderer)
{
	// mutex is automatically released when lock() goes out of scope
	std::lock_guard<std::mutex> lock(m_threadMutex);

	for (auto& chunk : m_chunks)
	{
		if ((chunk.second.m_hasMesh == true) && (chunk.second.m_isMarkedForDestruction == false))
		{
			chunk.second.addVaoToRenderer(renderer);
		}
	}
}

void WorldManager::generateWorld(Camera& camera, Renderer& renderer)
{
	int xCamera = static_cast<int>(std::floor(camera.getPosition().x));
	int zCamera = static_cast<int>(std::floor(camera.getPosition().z));
	int xCurrentCenterChunk = xCamera >= 0 ? xCamera / WorldConsts::CHUNKSIZE_X : xCamera / WorldConsts::CHUNKSIZE_X - 1;
	int zCurrentCenterChunk = zCamera >= 0 ? zCamera / WorldConsts::CHUNKSIZE_Z : zCamera / WorldConsts::CHUNKSIZE_Z - 1;
	m_centerChunk = xz_t(xCurrentCenterChunk, zCurrentCenterChunk);

	if (m_lastCenterChunk != m_centerChunk)	// chunks need to be deleted and generated
	{
		// mark chunks outside camera radius for destruction
		// if a chunk is marked in such a way, it can be replaced by a new one (done by loadChunks())
		// and should, until the new one is created, not be rendered
		this->markForDestruction();
	}

	// load chunks inside camera radius
	this->loadChunks();

	m_lastCenterChunk = m_centerChunk;
}

xz_t WorldManager::getArrayIndex(int x, int z)
{
	// this function enables a "rolling array" by calculating a modulo operation
	// we do not want to add and remove elements in m_chunks_tmp
	// but instead overwrite destroyed chunks with newly generated ones

	int x_tmp = x % m_worldSize;
	int z_tmp = z % m_worldSize;
	int x_t = x_tmp < 0 ? x_tmp + m_worldSize : x_tmp;
	int z_t = z_tmp < 0 ? z_tmp + m_worldSize : z_tmp;

	return xz_t(x_t, z_t);
}

void WorldManager::markForDestruction()
{
	if (m_centerChunk.first - m_lastCenterChunk.first > 0)	// camera moved along positive x-direction
	{
		for (int z = m_lastCenterChunk.second - m_worldSizeHalf; z <= m_lastCenterChunk.second + m_worldSizeHalf; ++z)
		{
			m_chunks[this->getArrayIndex(m_lastCenterChunk.first - m_worldSizeHalf, z)].m_isMarkedForDestruction = true;
		}
	}
	if (m_centerChunk.first - m_lastCenterChunk.first < 0)	// camera moved along negative x-direction
	{
		for (int z = m_lastCenterChunk.second - m_worldSizeHalf; z <= m_lastCenterChunk.second + m_worldSizeHalf; ++z)
		{
			m_chunks[this->getArrayIndex(m_lastCenterChunk.first + m_worldSizeHalf, z)].m_isMarkedForDestruction = true;
		}
	}
	if (m_centerChunk.second - m_lastCenterChunk.second > 0)	// camera moved along positive z-direction
	{
		for (int x = m_lastCenterChunk.first - m_worldSizeHalf; x <= m_lastCenterChunk.first + m_worldSizeHalf; ++x)
		{
			m_chunks[this->getArrayIndex(x, m_lastCenterChunk.second - m_worldSizeHalf)].m_isMarkedForDestruction = true;
		}
	}
	if (m_centerChunk.second - m_lastCenterChunk.second < 0)	// camera moved along negative z-direction
	{
		for (int x = m_lastCenterChunk.first - m_worldSizeHalf; x <= m_lastCenterChunk.first + m_worldSizeHalf; ++x)
		{
			m_chunks[this->getArrayIndex(x, m_lastCenterChunk.second + m_worldSizeHalf)].m_isMarkedForDestruction = true;
		}
	}
}

void WorldManager::loadChunks()
{
	for (int z = m_centerChunk.second - m_worldSizeHalf; z <= m_centerChunk.second + m_worldSizeHalf; ++z)
	{
		for (int x = m_centerChunk.first - m_worldSizeHalf; x <= m_centerChunk.first + m_worldSizeHalf; ++x)
		{
			xz_t key = this->getArrayIndex(x, z);

			if (m_chunks[key].m_isMarkedForDestruction == true)	// chunk does not yet exist
			{
				// generate new chunk and mesh it
				m_chunks[key].m_hasWorldData = false;
				m_chunks[key].addWorldData(m_world, xz_t(x, z));
				m_chunks[key].m_hasMesh = false;
				m_chunks[key].makeMesh();
				m_chunks[key].m_isMarkedForDestruction = false;
			}
		}
	}
}

void WorldManager::initializeChunkMap()
{
	for (int z = m_centerChunk.second - m_worldSizeHalf; z <= m_centerChunk.second + m_worldSizeHalf; ++z)
	{
		for (int x = m_centerChunk.first - m_worldSizeHalf; x <= m_centerChunk.first + m_worldSizeHalf; ++x)
		{
			xz_t key = this->getArrayIndex(x, z);
			m_chunks.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple());
			m_chunks[key].addWorldData(m_world, key);
			m_chunks[key].makeMesh();
			m_chunks[key].m_isMarkedForDestruction = true;
		}
	}
}