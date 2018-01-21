#include "WorldManager.h"

#include <chrono>

#include "Camera.h"

WorldManager::WorldManager(Camera& camera)
	: m_worldExists(true),
	m_worldSize(WorldConsts::WORLDSIZE),
	m_worldSizeHalf(WorldConsts::WORLDSIZE / 2)
{
	int xCamera = static_cast<int>(std::floor(camera.getPosition().x));
	int zCamera = static_cast<int>(std::floor(camera.getPosition().z));
	int xCurrentCenterChunk = xCamera >= 0 ? xCamera / WorldConsts::CHUNKSIZE_X : xCamera / WorldConsts::CHUNKSIZE_X - 1;
	int zCurrentCenterChunk = zCamera >= 0 ? zCamera / WorldConsts::CHUNKSIZE_Z : zCamera / WorldConsts::CHUNKSIZE_Z - 1;
}

WorldManager::~WorldManager()
{

}

void WorldManager::addToRenderer(Renderer& renderer)
{

}

void WorldManager::deleteWorld()
{
	
}

void WorldManager::generateWorld(Camera& camera, Renderer& renderer)
{

}
