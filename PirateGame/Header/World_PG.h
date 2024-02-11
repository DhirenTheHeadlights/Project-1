#pragma once

/// <summary>
/// This class is used to represent the world in the game.
/// </summary>

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
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
		World(sf::RenderWindow* window);
		~World() {};

		void setUpWorld();
		void createWorld(sf::Event event);
		void gameLoop();

	private:
		// Game objects
		Player player;
		Ship ship;
		View view;

		// Game state manager
		GameStateManager GSM;

		// Handlers
		std::unique_ptr<LandMassHandler> LMHandler;
		std::unique_ptr<MenuHandler> MH;

		// SFML Objects
		sf::RenderWindow* window = nullptr;
	};
}

