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
		~World() {};

		void createWorld(sf::Event event);
		void gameLoop();

	private:
		// SFML objects
		sf::RenderWindow& window;
		sf::Font font;
		sf::Clock mainClock;

		// Variables to store the world's values
		float width = 10000;
		float height = 10000;
		int cellSize = 10;
		sf::Vector2f worldMap = sf::Vector2f(width, height);

		// Game objects
		Player player;
		Ship ship = Ship(window);
		View view = View(window);
		Map map;

		// Handlers
		LandMassHandler LMHandler = LandMassHandler(map);
		GameStateManager GSM = GameStateManager();
		MenuHandler MH = MenuHandler(window, font, GSM);
	};
}

