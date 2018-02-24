#include "Camera.h"

#include "StaticFunctions.h"

void StaticFunctions::playerInput(sf::RenderWindow& window, Camera& camera)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		camera.processKeyboardInput(FORWARD);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		camera.processKeyboardInput(BACKWARD);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		camera.processKeyboardInput(LEFT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		camera.processKeyboardInput(RIGHT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		camera.processKeyboardInput(UP);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))	// reset camera position and view direction
	{
		camera.resetPosition();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))	// reset view direction
	{
		camera.resetView();
	}

	sf::Vector2i currentMousePosition(sf::Mouse::getPosition(window));
	sf::Vector2i centerMousePosition(window.getSize().x / 2, window.getSize().y / 2);
	GLfloat xOffset = static_cast<GLfloat> (currentMousePosition.x - centerMousePosition.x);
	GLfloat yOffset = static_cast<GLfloat> (centerMousePosition.y - currentMousePosition.y);
	sf::Mouse::setPosition(centerMousePosition, window);
	camera.processMouseMovement(xOffset, yOffset);
}