#pragma once

/// This class is used to represent the world in the game.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "GlobalFontHandler_PG.h"
#include "GlobalMap_PG.h"
#include "GlobalGameStateManager_PG.h"
#include "GlobalInputHandler_PG.h"
#include "GlobalWindController_PG.h"


#include "Player_PG.h"
#include "View_PG.h"
#include "PlayerShip_PG.h"
#include "EnemyShipHandler_PG.h"
#include "LandMassHandler_PG.h"
#include "CollisionManager_PG.h"
#include "MenuHandler_PG.h"

namespace PirateGame {
	class World {
	public:
		World(sf::RenderWindow* window, bool debug = false);
		~World() {};

		void createWorld(sf::Event event);

	private:
		// Functions
		void setUpWorld();
		void gameLoop();
		void drawGameLoop();

		// Game objects
		Player player;
		std::unique_ptr<PlayerShip> playerShip;
		View view;

		// Handlers
		std::unique_ptr<LandMassHandler> LMHandler;
		std::unique_ptr<MenuHandler> MH;
		std::unique_ptr<CollisionManager> CM;
		std::unique_ptr<EnemyShipHandler> ESH;

		// SFML Objects
		sf::RenderWindow* window = nullptr;
		sf::RectangleShape background;
		sf::Color backgroundColor = sf::Color(0, 158, 163, 255);

		// Frame rate variables
		sf::Clock frameRateClock;
		sf::Time frameRateUpdateTime;
		sf::Text frameRateText;
		int frameCount = 0;
		const float numFramesToAverage = 40.f;

		// Debug
		bool debug = true;

		// Object numbers [not debug]
		int numLandMasses = 5;
		float distanceBetweenLandMasses = 2000.f;
		int numEnemyShips = 5;
		float distanceBetweenEnemyShips = 1000.f;

		// Object numbers [debug]
		int numLandMassesDebug = 0;
		float distanceBetweenLandMassesDebug = 100.f;
		int numEnemyShipsDebug = 10;
		float distanceBetweenEnemyShipsDebug = 100.f;
	};
}

