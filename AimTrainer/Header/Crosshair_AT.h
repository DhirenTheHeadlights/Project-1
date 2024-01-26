#pragma once
#include <SFML/Graphics.hpp>

class Crosshair {
public:
	Crosshair(sf::RenderWindow& window);
	void setLength(float length);
	void setHeight(float height);
	void setThickness(float thickness);
	void setColour(sf::Color colour);
	void setPosition();
	void draw(bool visible);
private:
	sf::RenderWindow& window;
	sf::RectangleShape horizontal;
	sf::RectangleShape vertical;

	float length = 100;
	float height = 100;
	float thickness = 1;
	sf::Color colour = sf::Color::Black;
	sf::Vector2f mousePos;
};
