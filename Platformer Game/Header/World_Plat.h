#include "SFML/Graphics.hpp"
#include <iostream>
#include "Map.h"

#include "Platforms_Plat.h"
#include "Player_Plat.h"
#pragma once

namespace PlatformerGame {
	class World {
	public:
		// World.h
		World(sf::RenderWindow& window);
		void gameLoop(sf::Event event);


	private:

		// Define the SFML window reference
		sf::RenderWindow& window;

		// Define the player
		Player player;

		//Define the platforms
		Platform_Plat platforms;

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
}