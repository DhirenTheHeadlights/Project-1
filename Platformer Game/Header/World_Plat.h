#include "SFML/Graphics.hpp"
#include <iostream>
#include "Map.h"

#include "Platforms_Plat.h"
#include "Player_Plat.h"
#include "HUD_Plat.h"
#pragma once

namespace PlatformerGame {
	class World {
	public:
		// World.h
		World(sf::RenderWindow& window);
		void gameLoop(sf::Event event);
		void createWorld(sf::Event event);

		HealthBar healthBar;


	private:

		// Define the SFML window reference
		sf::RenderWindow& window;

		// Define the player
		Player player;

		//Define the platforms
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

		sf::RectangleShape background;

	};
}