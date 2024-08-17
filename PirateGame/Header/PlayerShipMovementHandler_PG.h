#pragma once

/// Derived player movement handler class

#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class PlayerShipMovementHandler : public ShipMovementHandler {
	public:
		PlayerShipMovementHandler(sf::RenderWindow* window, sf::Sprite& sprite, float& baseSpeed) : ShipMovementHandler(sprite, baseSpeed), window(window) {};
		~PlayerShipMovementHandler() {};

		// Movement functions
		void update(sf::Vector2f sailDirection, sf::Time dt, sf::Vector2f windDirection, float windSpeed) override;

	private:
		void setSpriteRotation() override;

		sf::RenderWindow* window;
	};
}

