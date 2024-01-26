#pragma once

#include <SFML/Graphics.hpp>

/// <summary>
/// This class is used to represent the view in the game.
/// The view is the part of the game that the player can see.
/// </summary>

class View {
public:
	View(sf::RenderWindow& window);
	~View();
	void setCenter(sf::Vector2f position);
private:
	sf::View view;
	sf::RenderWindow& window;
};

