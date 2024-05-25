#pragma once

/// Derived player movement handler class

#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class PlayerShipMovementHandler : public ShipMovementHandler {
	public:
		PlayerShipMovementHandler(sf::Sprite& sprite) : ShipMovementHandler(sprite) {};
		~PlayerShipMovementHandler() {};

		// Movement functions
		void update(float baseSpeed, sf::Vector2f sailDirection) override;

	private:
		void setSpriteRotation() override;
	};
}

