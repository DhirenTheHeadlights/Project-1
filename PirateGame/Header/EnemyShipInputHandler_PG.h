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
		void setTargetPos(sf::Vector2f playerPos) { this->playerPos = playerPos; }
		void setFiringDistance(float firingDistance) { this->firingDistance = firingDistance; }
	private:
		sf::Vector2f playerPos;
		float firingDistance = 0.f;
	};
}
