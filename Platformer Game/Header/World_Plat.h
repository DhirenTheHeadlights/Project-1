#include "SFML/Graphics.hpp"
#include <iostream>


#include "Player_Plat.h"

class world_plat {
public:
	// World.h
	world_plat(sf::RenderWindow& window);
	void gameLoop(sf::Event event);


private:

	// Define the SFML window reference
	sf::RenderWindow& window;

	// Define the player
	Player_Plat player;

	//variables for movement
	bool isJumping = false;
	sf::Vector2f velocity;

	// Map
	float len = window.getSize().x;
	float wid = window.getSize().y;
	sf::Vector2f map = sf::Vector2f(len, wid);

};
#pragma once
