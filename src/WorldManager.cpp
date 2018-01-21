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

	m_thread.emplace_back([&]() {
		while (m_worldExists)
		{
			this->generateWorld(camera, renderer);
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	});
	std::cout << "THREAD::World generation thread launched." << std::endl;
}

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
		std::cout << "Number of chunks in std::map: " << m_chunks.size() << std::endl;
	}
}

void WorldManager::addToRenderer(Renderer& renderer)
{
	std::lock_guard<std::mutex> lock(m_threadMutex);	// mutex is automatically released when lock() goes out of scope

	for (auto& chunk : m_chunks)
	{
		if ((chunk.second.m_hasMesh == true) && (chunk.second.m_isMarkedForDestruction == false))
		{
			chunk.second.addVaoToRenderer(renderer);
		}
	}
}

void WorldManager::deleteWorld()
{
	std::lock_guard<std::mutex> lock(m_threadMutex);

	if (m_chunks.size() > 0)
	{
		for (std::map<xz_t, Chunk>::iterator it = m_chunks.begin(); it != m_chunks.end();)
		{
			if (it->second.m_isMarkedForDestruction == true)
			{
				it = m_chunks.erase(it);
			}
			else
			{
				++it;
			}
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

	// load chunks inside camera radius
	for (int z = m_centerChunk.second - m_worldSizeHalf; z <= m_centerChunk.second + m_worldSizeHalf; ++z)
	{
		for (int x = m_centerChunk.first - m_worldSizeHalf; x <= m_centerChunk.first + m_worldSizeHalf; ++x)
		{
			xz_t key = xz_t(x, z);

			if (m_chunks.count(key) == 0)	// chunk does not yet exist
			{
				// equivalent to m_chunks[key];:
				m_chunks.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple());
			}

			if (m_chunks.at(key).m_hasWorldData == false)
			{
				m_chunks.at(key).addWorldData(m_world, key);
			}

			if (m_chunks.at(key).m_hasMesh == false)
			{
				m_chunks.at(key).makeMesh();
			}
		}
	}

	// remove chunks outside of camera radius
	if (m_lastCenterChunk == m_centerChunk)
	{
		return;
	}
	if (m_centerChunk.first - m_lastCenterChunk.first > 0)	// camera moved along positive x-direction
	{
		for (int z = m_lastCenterChunk.second - m_worldSizeHalf; z <= m_lastCenterChunk.second + m_worldSizeHalf; ++z)
		{
			m_chunks.at(xz_t(m_lastCenterChunk.first - m_worldSizeHalf, z)).m_isMarkedForDestruction = true;
		}
	}
	if (m_centerChunk.first - m_lastCenterChunk.first < 0)	// camera moved along negative x-direction
	{
		for (int z = m_lastCenterChunk.second - m_worldSizeHalf; z <= m_lastCenterChunk.second + m_worldSizeHalf; ++z)
		{
			m_chunks.at(xz_t(m_lastCenterChunk.first + m_worldSizeHalf, z)).m_isMarkedForDestruction = true;
		}
	}
	if (m_centerChunk.second - m_lastCenterChunk.second > 0)	// camera moved along positive z-direction
	{
		for (int x = m_lastCenterChunk.first - m_worldSizeHalf; x <= m_lastCenterChunk.first + m_worldSizeHalf; ++x)
		{
			m_chunks.at(xz_t(x, m_lastCenterChunk.second - m_worldSizeHalf)).m_isMarkedForDestruction = true;
		}
	}
	if (m_centerChunk.second - m_lastCenterChunk.second < 0)	// camera moved along negative z-direction
	{
		for (int x = m_lastCenterChunk.first - m_worldSizeHalf; x <= m_lastCenterChunk.first + m_worldSizeHalf; ++x)
		{
			m_chunks.at(xz_t(x, m_lastCenterChunk.second + m_worldSizeHalf)).m_isMarkedForDestruction = true;
		}
	}

	m_lastCenterChunk = m_centerChunk;
}
