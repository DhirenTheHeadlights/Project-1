#pragma once

/// This class contains the methods for handling the input
/// for the ship. This includes shooting, etc.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "GlobalMap_PG.h"
#include "GlobalInputHandler_PG.h"

#include "ShipCannonHandler_PG.h"
#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class ShipInputHandler {
	public:
		ShipInputHandler(sf::Sprite& sprite) : sprite(sprite) {};
		~ShipInputHandler() {};

		void update();
		void draw();
		virtual void handleCannonFire() = 0;
		virtual void handleCannonAim() = 0;
		virtual void handleAnchorDrop() = 0;

		// Setters
		void setNumCannons(int numCannons) { this->baseNumCannons = numCannons; }
		void setBaseSpeed(float baseSpeed) { this->baseSpeed = baseSpeed; }
		void setCannonHandler(ShipCannonHandler* SCH) { this->SCH = SCH; }
		void setMovementHandler(ShipMovementHandler* SMH) { this->SMH = SMH; }

		// Getters
		ShipCannonHandler* getCannonHandler() { return SCH; }
	    ShipMovementHandler* getMovementHandler() { return SMH; }

	protected:
		sf::Sprite& sprite;

		// Handlers
		ShipCannonHandler* SCH = nullptr;
		ShipMovementHandler* SMH = nullptr;

		// Ship properties
		int baseNumCannons = 1;
		int numCannons = 1;
		float baseSpeed = 1.f;
	};
}
