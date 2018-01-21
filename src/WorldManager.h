#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include "World.h"

class Camera;
class Renderer;

class WorldManager
{
public:
	WorldManager() = delete;
	WorldManager(Camera&);
	~WorldManager();

	void addToRenderer(Renderer&);
	void deleteWorld();
	void generateWorld(Camera&, Renderer&);

	bool m_worldExists;
	const int m_worldSize;
	const int m_worldSizeHalf;

private:
	World m_world;
};

#endif // !WORLDMANAGER_H
