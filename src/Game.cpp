#include "Game.h"

#include <iostream>

#include <SFML\Graphics.hpp>
#include <GL\glew.h>
#include <glm\glm.hpp>

Game::Game()
	: m_camera(GameConsts::WINDOW_WIDTH, GameConsts::WINDOW_HEIGHT)
{
	std::cout << "Game created." << std::endl;
	this->initialize();
}

void Game::run()
{
	sf::Clock deltaClock;
	sf::Clock fpsClock;
	static int counter = 0;
	float fps = 0;
	int iFps = 0;

	while (m_window->isOpen())
	{
		fps += fpsClock.restart().asSeconds();
		m_deltaTime = deltaClock.restart().asSeconds();

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

		m_window->display();
	}
}

void Game::initialize()
{
	sf::ContextSettings contextSettings;
	contextSettings.depthBits = 24;	// bits of depth buffer
	contextSettings.stencilBits = 8;	// bits of stencil buffer
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(GameConsts::WINDOW_WIDTH, GameConsts::WINDOW_HEIGHT), "MeinCraft", sf::Style::Close | sf::Style::Titlebar, contextSettings);
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

	m_deltaTime = 0.f;	// makes camera velocity independent of frame rate

	m_worldMgr = std::make_unique<WorldManager>(m_camera);
}

void Game::processEvents()
{
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

}