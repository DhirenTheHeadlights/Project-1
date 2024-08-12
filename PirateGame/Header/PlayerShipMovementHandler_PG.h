#pragma once

/// Derived player movement handler class

#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class PlayerShipMovementHandler : public ShipMovementHandler {
	public:
		PlayerShipMovementHandler(sf::RenderWindow* window, sf::Sprite& sprite) : ShipMovementHandler(sprite), window(window) {};
		~PlayerShipMovementHandler() {};

		// Movement functions
		void update(float baseSpeed, sf::Vector2f sailDirection, float dt, sf::Vector2f windDirection, float windSpeed) override;

	private:
		void setSpriteRotation() override;

		sf::RenderWindow* window;
	};
}

