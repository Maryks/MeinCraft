#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <SFML\Graphics.hpp>

#include <map>
#include <memory>
#include <string>

namespace Fonts
{
	enum Typeface
	{
		Arial
	};
}

class FontManager
{
public:
	FontManager();

	const sf::Font& getFont(Fonts::Typeface);

private:
	void load(Fonts::Typeface, const std::string&);
	
	std::map<Fonts::Typeface, std::unique_ptr<sf::Font>> m_fonts;
};


#endif // !FONTMANAGER_H
