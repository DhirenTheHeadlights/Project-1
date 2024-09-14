#pragma once

/// Derived player movement handler class

#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class PlayerShipMovementHandler : public ShipMovementHandler {
	public:
		PlayerShipMovementHandler(sf::RenderWindow* window, sf::Sprite& sprite, float& baseSpeed, JSONLoader* json ) : ShipMovementHandler(sprite, baseSpeed, json), window(window) {};
		~PlayerShipMovementHandler() {};

		// Movement functions
		void update(const sf::Vector2f& sailDirection, const sf::Time& dt, const sf::Vector2f& windDirection, const float windSpeed) override;

	private:
		void setSpriteRotation() override;

		sf::RenderWindow* window;
	};
}

