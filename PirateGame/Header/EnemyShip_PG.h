#pragma once

/// This class is almost identical to PlayerShip, but it is used for the enemy ships.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <unordered_map>

#include "GlobalValues_PG.h"
#include "Ship_PG.h"
#include "EnemyShipInputHandler_PG.h"
#include "EnemyShipMovementHandler_PG.h"

namespace PirateGame {
	class EnemyShip : public Ship {
	public:
		EnemyShip() {};
		~EnemyShip() {};

		// Custom functions
		void customShipSetUp() override;
		void customShipUpdate() override;
		void customShipDraw() override;

		// Getters
		bool isActive() const { return active; }
		EnemyShipMovementHandler& getMovementHandler() { return *SMH; }
		EnemyShipInputHandler& getInputHandler() { return *SIH; }

	private:
		// Health bars
		sf::RectangleShape healthBarRed;
		sf::RectangleShape healthBarGreen;

		// Active flag
		bool active = false;

		// Handlers
		std::unique_ptr<EnemyShipInputHandler> SIH;
		std::unique_ptr<EnemyShipMovementHandler> SMH;
	};
}

