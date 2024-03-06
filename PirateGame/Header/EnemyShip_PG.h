#pragma once

/// This class is almost identical to PlayerShip, but it is used for the enemy ships.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <unordered_map>

#include "GlobalValues_PG.h"
#include "Ship_PG.h"

namespace PirateGame {
	class EnemyShip : public Ship {
	public:
		// Custom functions
		void customShipSetUp() override;
		void customShipUpdate() override;
		void customShipDraw() override;
	private:
		// Health bars
		sf::RectangleShape healthBarRed;
		sf::RectangleShape healthBarGreen;
	};
}

