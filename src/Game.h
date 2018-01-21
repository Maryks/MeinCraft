#ifndef GAME_H
#define GAME_H

#include <memory>

#include <GL\glew.h>
#include <SFML\Graphics.hpp>

#include "Camera.h"
#include "WorldManager.h"

namespace GameConsts
{
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
};

class Game
{
public:
	Game();

	void run();

private:
	void initialize();
	void processEvents();
	void render();

	GLfloat m_deltaTime;
	Camera m_camera;
	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<WorldManager> m_worldMgr;
};

#endif // !GAME_H
