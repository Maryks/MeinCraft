#include <iostream>

#include "FontManager.h"

FontManager::FontManager()
{
	std::string path = "src/Fonts/";

	this->load(Fonts::Arial, path + "arial.ttf");

	std::cout << "FontManager created." << std::endl;
}

const sf::Font& FontManager::getFont(Fonts::Typeface typeface)
{
	auto found = m_fonts.find(typeface);
	return *found->second;
}

void FontManager::load(Fonts::Typeface typeface, const std::string& filename)
{
	auto font = std::make_unique<sf::Font>();
	if (!font->loadFromFile(filename))
	{
		std::cout << "FontManager: Failed to load from file " + filename << std::endl;
	}
	m_fonts.insert(std::make_pair(typeface, std::move(font)));
}