#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"

class Debug {
public:
	Debug(sf::RenderWindow& window);
	void drawInformation(sf::RenderWindow& window, const std::string& info, int textSize, float setPosX, float setPosY);
	void drawVector(const sf::Vector2f& start, const sf::Vector2f& direction, sf::RenderWindow& window, float magnitude, sf::Color color);
	void drawCell(int x, int y, Map& map, sf::Color color);
private:
	sf::RenderWindow& window;
	sf::Font font;
};
