#include <SFML\Graphics.hpp>
#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Game.h"

int main()
{
	sf::Context context;
	glewInit();

	Game game;
	game.run();

	return 0;
}
