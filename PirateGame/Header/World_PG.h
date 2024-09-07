#pragma once

/// This class is used to represent the world in the game.

#include <SFML/Graphics.hpp>

#include "GlobalContext_PG.h"

#include "CollisionManager_PG.h"
#include "EnemyShipHandler_PG.h"
#include "JSONSave_PG.h"
#include "LandMassHandler_PG.h"
#include "MenuHandler_PG.h"
#include "PlayerShip_PG.h"
#include "Player_PG.h"
#include "View_PG.h"
#include "WaterTiler_PG.h"

namespace PirateGame {
	class World {
	public:
		World(sf::RenderWindow* window) : window(window) {};
		~World() {};

		virtual void setUpWorld();

		virtual void createWorld(sf::Event event);
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
		virtual void updateCoreElements();

		// Global context
		GlobalContext context;
		std::shared_ptr<GlobalQuadtreeHandler> GQH = std::make_shared<GlobalQuadtreeHandler>();

		// Game objects
		Player player;
		std::unique_ptr<PlayerShip> playerShip;
		ShipClass playerShipClass = ShipClass::ManOWar;
		View view;

		// Handlers
		LandMassHandler LMH = LandMassHandler(context, GQH.get());
		MenuHandler MH = MenuHandler(context);
		CollisionManager CM = CollisionManager(context);
		EnemyShipHandler ESH = EnemyShipHandler(context, GQH.get());

		// SFML Objects
		sf::RenderWindow* window = nullptr;

		// Background
		WaterTiler waterTiler;

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

