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
		World(sf::RenderWindow* window) : window(window) {};
		~World() {};

		virtual void setUpWorld();

		virtual void createWorld(sf::Event event) = 0;
	protected:
		// Functions
		virtual void setUpWorldElements();
		virtual void setUpPlayerShip();
		virtual void setUpLandMasses();
		virtual void setUpEnemyShips();
		virtual void setUpCollisionManager();
		virtual void setUpMenus();
		virtual void setUpUI();

		virtual void updateGameLoop(sf::Event event) = 0;
		virtual void drawGameLoop();

		// Game objects
		Player player;
		std::unique_ptr<PlayerShip> playerShip;
		View view;

		// Handlers
		LandMassHandler LMH;
		MenuHandler MH;
		CollisionManager CM;
		EnemyShipHandler ESH;

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

		// Game loop clock variables
		sf::Clock gameLoopClock;
		sf::Time gameLoopWaitTime = sf::milliseconds(10);

		// Exp text temp
		sf::Text experience;

		// Object numbers
		int numLandMasses = 0;
		float distanceBetweenLandMasses = 0.f;
		int numEnemyShips = 0;
		float distanceBetweenEnemyShips = 0.f;
	};
}

