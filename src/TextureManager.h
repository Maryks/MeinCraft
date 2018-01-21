#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML\Graphics.hpp>

#include <map>
#include <memory>
#include <string>

namespace Textures
{
	enum ID
	{
		Sheet,
		nofTextures	// number of textures
	};
}

class TextureManager
{
public:
	TextureManager();

	void bind(Textures::ID);

private:
	const sf::Texture& get(Textures::ID);
	void load(Textures::ID, const std::string&);

	std::map<Textures::ID, std::unique_ptr<sf::Texture>> m_textures;
};

#endif // !TEXTUREMANAGER_H
