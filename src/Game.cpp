#include <SFML\Graphics.hpp>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "BlockData.h"
#include "StaticFunctions.h"

#include "Game.h"

Game::Game()
	: m_camera(GameConsts::WINDOW_WIDTH, GameConsts::WINDOW_HEIGHT)
{
	std::cout << "Game created." << std::endl;
	this->initializeWindow();

	m_worldMgr = std::make_unique<WorldManager>(m_camera, m_renderer);
}

void Game::run()
{
	sf::Clock deltaClock;
	sf::Clock fpsClock;
	sf::Clock fps2Clock;
	double deltaTime = 0;
	static int counter = 0;
	static int counter2 = 0;
	float fps = 0;
	float fps2 = 0;
	int iFps = 0;
	float secondsBetweenGameUpdates = 1.f / 60;	// makes camera velocity independent of frame rate

	while (m_window->isOpen())
	{
		fps += fpsClock.restart().asSeconds();
		fps2 += fps2Clock.restart().asSeconds();

		// update the camera every 'secondsBetweenGameUpdates' seconds
		deltaTime += deltaClock.restart().asSeconds();
		if (deltaTime >= secondsBetweenGameUpdates)
		{
			StaticFunctions::playerInput(*m_window, m_camera);
			deltaTime -= secondsBetweenGameUpdates;
		}

		this->processEvents();
		this->render();

		// moving average fps
		if (counter == 10)
		{
			iFps = static_cast<int>(counter * 1.f / fps);
			counter = 0;
			fps = 0;
		}
		++counter;

		if (counter2 == 100)
		{
			std::cout << "\r FPS: " << static_cast<int>(counter2 * 1.f / fps2) << " ";
			counter2 = 0;
			fps2 = 0;
		}
		++counter2;

		int xCamera = static_cast<int>(std::floor(m_camera.getPosition().x));
		int yCamera = static_cast<int>(std::floor(m_camera.getPosition().y));
		int zCamera = static_cast<int>(std::floor(m_camera.getPosition().z));
		int xChunk = xCamera >= 0 ? xCamera / WorldConsts::CHUNKSIZE_X : xCamera / WorldConsts::CHUNKSIZE_X - 1;
		int zChunk = zCamera >= 0 ? zCamera / WorldConsts::CHUNKSIZE_Z : zCamera / WorldConsts::CHUNKSIZE_Z - 1;
		m_renderer.addText("FPS: " + std::to_string(iFps), 10.f, 10.f);
		m_renderer.addText("Camera X: " + std::to_string(xCamera), 10.f, 40.f);
		m_renderer.addText("Camera Y: " + std::to_string(yCamera), 10.f, 70.f);
		m_renderer.addText("Camera Z: " + std::to_string(zCamera), 10.f, 100.f);
		m_renderer.addText("Chunk X: " + std::to_string(xChunk), 250.f, 10.f);
		m_renderer.addText("Chunk Z: " + std::to_string(zChunk), 250.f, 40.f);

		m_renderer.displayText(*m_window);
		m_window->display();
	}
}

void Game::initializeWindow()
{
	sf::ContextSettings contextSettings;
	contextSettings.depthBits = 24;	// bits of depth buffer
	contextSettings.stencilBits = 8;	// bits of stencil buffer
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(GameConsts::WINDOW_WIDTH, GameConsts::WINDOW_HEIGHT), "MineCraft", sf::Style::Close | sf::Style::Titlebar, contextSettings);
	m_window->setPosition(sf::Vector2i(500, 50));
	//m_window->setVerticalSyncEnabled(true);
	m_window->setMouseCursorVisible(false);
	m_window->setMouseCursorGrabbed(true);

	// set mouse cursor to center of screen
	sf::Mouse::setPosition(sf::Vector2i(m_window->getSize().x / 2, m_window->getSize().y / 2), *m_window);

	//glewExperimental = GL_TRUE;	// support for experimental or pre-release graphics drivers
	glewInit();
	glViewport(0, 0, m_window->getSize().x, m_window->getSize().y);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.f, (float)m_window->getSize().x / (float)m_window->getSize().y, 1.f, 1000.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);	// tell OpenGL to use textures when drawing
}

void Game::processEvents()
{
	static bool wireframeMode = true;	// gl polygone mode

	sf::Event inputEvent;	// flush queue every frame
	while (m_window->pollEvent(inputEvent))
	{
		switch (inputEvent.type)
		{
		case sf::Event::KeyPressed:
			//keyCallback(inputEvent.key.code);
			// it would improve performance if playerInput() was replaced by something like keyCallback(); bool key[code] ...
			if (inputEvent.key.code == sf::Keyboard::Escape)
			{
				m_window->close();
			}
			if (inputEvent.key.code == sf::Keyboard::Z)
			{
				if (wireframeMode)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					wireframeMode = false;
				}
				else
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					wireframeMode = true;
				}
			}
			break;
		case sf::Event::MouseWheelScrolled:
			//scrollCallback(inputEvent.mouseWheelScroll.delta);
			m_camera.processMouseScroll(inputEvent.mouseWheelScroll.delta);
			break;
		default:
			break;
		}
	}
}

void Game::render()
{
	m_window->setActive(true);

	glClearColor(0.0, 0.0, 0.0, 1.0);	// black
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_worldMgr->addToRenderer(m_renderer);
	m_renderer.draw(m_camera);
}
