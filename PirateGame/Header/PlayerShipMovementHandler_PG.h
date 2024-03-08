#pragma once

/// Derived player movement handler class

#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class PlayerShipMovementHandler : public ShipMovementHandler {
	public:
		PlayerShipMovementHandler(sf::Sprite& sprite) : sprite(sprite), ShipMovementHandler(sprite) {};
		~PlayerShipMovementHandler() {};

		// Movement functions
		void move(float baseSpeed) override;

	private:
		void updateVelocity(const sf::Vector2f& direction, float elapsedTime, const float baseSpeed) override;
		void setSpriteRotation(sf::Vector2f& direction) override;
	};
}

