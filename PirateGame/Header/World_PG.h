#pragma once

/// This class is used to represent the world in the game.

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

		void createWorld(sf::Event event);

	private:
		// Functions
		void setUpWorld();
		void gameLoop();
		void drawGameLoop();

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
		sf::RectangleShape background;

		// Frame rate variables
		sf::Clock frameRateClock;
		sf::Time frameRateUpdateTime;
		sf::Text frameRateText;
		int frameCount = 0;
		const float numFramesToAverage = 40.f;
	};
}

