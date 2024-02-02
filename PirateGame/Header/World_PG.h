#pragma once

/// <summary>
/// This class is used to represent the world in the game.
/// </summary>

#include <SFML/Graphics.hpp>

#include "Player_PG.h"
#include "View_PG.h"
#include "Ship_PG.h"
#include "LandMassHandler_PG.h"
#include "GameStateManager_PG.h"
#include "MenuHandler_PG.h"
#include "Map.h"

namespace PirateGame {
	class World {
	public:
		World(sf::RenderWindow& window);
		~World();

		void createWorld(sf::Event event);
		void gameLoop();

	private:
		// Variables to store the world's values
		int width = 10000;
		int height = 10000;
		int cellSize = 10;

		// Map size
		sf::Vector2f worldMap = sf::Vector2f(width, height);

		// Main clock
		sf::Clock mainClock;

		// This will be implemented later
		Player player;

		// This will be implemented later
		Ship ship = Ship(window);

		// This will be implemented later
		View view = View(window);

		// LandMass Handler
		LandMassHandler LMHandler = LandMassHandler(map);

		// GameState Manager
		GameStateManager GSM;

		// Menu Handler
		MenuHandler MH = MenuHandler(window, font, GSM);

		// This will be implemented later
		sf::RenderWindow& window;

		// Map
		Map map;
	};
}

