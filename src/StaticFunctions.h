#ifndef STATICFUNCTIONS_H
#define STATICFUNCTIONS_H

#include <SFML\Graphics.hpp>

class Camera;

class StaticFunctions
{
public:
	static void playerInput(sf::RenderWindow& window, Camera& camera);
};

#endif // !STATICFUNCTIONS_H
