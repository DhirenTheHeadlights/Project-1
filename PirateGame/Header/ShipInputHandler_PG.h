#pragma once

/// This class contains the methods for handling the input
/// for the ship. This includes shooting, etc.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "ShipCannonHandler_PG.h"
#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class ShipInputHandler {
	public:
		ShipInputHandler(sf::Sprite& sprite) : sprite(sprite) {};
		~ShipInputHandler() {};

		void update();
		void draw();
		void handleCannonFire();
		void handleCannonAim();
		void handleAnchorDrop();

		// Setters
		void setNumCannons(int numCannons) { this->numCannons = numCannons; }
		void setBaseSpeed(float baseSpeed) { this->baseSpeed = baseSpeed; }
		void setCannonHandler(ShipCannonHandler* SCH) { this->SCH = SCH; }
		void setMovementHandler(ShipMovementHandler* SMH) { this->SMH = SMH; }

		// Getters
		ShipCannonHandler* getCannonHandler() { return SCH; }
		ShipMovementHandler* getMovementHandler() { return SMH; }
		std::string const getFireKeyString() { return GlobalValues::getInstance().keyToString(fireKey); }
		std::string const getManualAimKeyString() { return GlobalValues::getInstance().keyToString(manualAimKey); }
		std::string const getPortMouseButtonString() { return GlobalValues::getInstance().buttonToString(portMouseButton); }
		std::string const getStarboardMouseButtonString() { return GlobalValues::getInstance().buttonToString(starboardMouseButton); }
	private:
		sf::Sprite& sprite;

		// Handlers
		ShipCannonHandler* SCH = nullptr;
		ShipMovementHandler* SMH = nullptr;

		// Ship properties
		int numCannons = 1;
		float baseSpeed = 1.f;

		// Keys
		sf::Keyboard::Key fireKey = sf::Keyboard::Space;
		sf::Keyboard::Key manualAimKey = sf::Keyboard::E;

		sf::Mouse::Button portMouseButton = sf::Mouse::Left;
		sf::Mouse::Button starboardMouseButton = sf::Mouse::Right;
	};
}
