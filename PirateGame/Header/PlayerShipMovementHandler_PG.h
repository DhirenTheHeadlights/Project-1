#pragma once

/// Derived player movement handler class

#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class PlayerShipMovementHandler : public ShipMovementHandler {
	public:
		PlayerShipMovementHandler(sf::Sprite& sprite) : ShipMovementHandler(sprite) {};
		~PlayerShipMovementHandler() {};

		// Movement functions
		void move(float baseSpeed) override;

	private:
		void setSpriteRotation() override;


		float turningSpeed = 0.1f;
	};
}

