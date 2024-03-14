#pragma once

/// Derived enemy movement handler class

#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class EnemyShipMovementHandler : public ShipMovementHandler {
	public:
		EnemyShipMovementHandler(sf::Sprite& sprite) : ShipMovementHandler(sprite) {};
		~EnemyShipMovementHandler() {};

		// Movement functions
		void move(float baseSpeed) override;
		void setPlayerPosition(sf::Vector2f playerPos) { this->playerPos = playerPos; }
		void setPlayerVelocity(sf::Vector2f playerVelocity) { this->playerVelocity = playerVelocity; }
	private:
		void setSpriteRotation() override;
		sf::Vector2f playerPos;	
		sf::Vector2f playerVelocity;
		float turningSpeed = 0.1f;
	};
}