#pragma once

/// This class contains the methods for handling the input for enemy ships.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "ShipInputHandler_PG.h"
#include "ShipCannonHandler_PG.h"
#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class EnemyShipInputHandler : public ShipInputHandler {
	public:
		EnemyShipInputHandler(sf::Sprite& sprite) : ShipInputHandler(sprite) {};
		~EnemyShipInputHandler() {};

		void handleCannonFire() override;
		void handleCannonAim() override;
		void handleAnchorDrop() override;

		// Setters
		void setPlayerPos(sf::Vector2f playerPos) { this->playerPos = playerPos; }
	private:
		sf::Vector2f playerPos;
		float firingDistance = 1000.f;
	};
}
