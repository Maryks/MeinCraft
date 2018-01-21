#include <iostream>

#include "TextureManager.h"

TextureManager::TextureManager()
{
	std::string path = "src/Textures/";

	this->load(Textures::Sheet, path + "sheet.png");

	std::cout << "TextureManager created." << std::endl;
}

void TextureManager::bind(Textures::ID id)
{
	sf::Texture::bind(&this->get(id));
}

const sf::Texture& TextureManager::get(Textures::ID id)
{
	auto found = m_textures.find(id);
	return *found->second;
}

void TextureManager::load(Textures::ID id, const std::string& filename)
{
	auto texture = std::make_unique<sf::Texture>();
	if (!texture->loadFromFile(filename))
	{
		std::cout << "TextureManager: Failed to load from file " + filename << std::endl;
	}
	m_textures.insert(std::make_pair(id, std::move(texture)));
}