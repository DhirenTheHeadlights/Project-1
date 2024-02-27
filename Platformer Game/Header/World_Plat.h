#include "SFML/Graphics.hpp"
#include <iostream>
#include "Map.h"


#include "Platforms_Plat.h"
#include "Player_Plat.h"
#pragma once
class world_plat {
public:
	// World.h
	world_plat(sf::RenderWindow& window);
	void gameLoop(sf::Event event);
	void createGame(sf::Event event);


private:

	// Define the SFML window reference
	sf::RenderWindow& window;

	// Define the player
	Player_Plat player;

	//Define Platforms
	Platforms_Plat platforms;

	//variables for movement
	bool isJumping = false;
	sf::Vector2f velocity;

	// Map
	float len = 100000;
	float wid = 1000;
	sf::Vector2f worldMap = sf::Vector2f(len, wid);

	Map map;

	sf::View view;

};

