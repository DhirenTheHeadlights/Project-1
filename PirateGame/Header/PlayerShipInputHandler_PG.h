#pragma once

/// Child class of ShipInputHandler. This class if for player ship input handling.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "ShipInputHandler_PG.h"

namespace PirateGame {
	class PlayerShipInputHandler : public ShipInputHandler {
	public:
		PlayerShipInputHandler(sf::Sprite& sprite) : ShipInputHandler(sprite) {};
		~PlayerShipInputHandler() {};

		void handleCannonFire() override;
		void handleCannonAim() override;
		void handleAnchorDrop() override;

		std::string const getFireKeyString() { return GlobalValues::getInstance().keyToString(fireKey); }
		std::string const getManualAimKeyString() { return GlobalValues::getInstance().keyToString(manualAimKey); }
		std::string const getPortMouseButtonString() { return GlobalValues::getInstance().buttonToString(portMouseButton); }
		std::string const getStarboardMouseButtonString() { return GlobalValues::getInstance().buttonToString(starboardMouseButton); }
	private:
		// Keys
		sf::Keyboard::Key fireKey = sf::Keyboard::Space;
		sf::Keyboard::Key manualAimKey = sf::Keyboard::E;

		sf::Mouse::Button portMouseButton = sf::Mouse::Left;
		sf::Mouse::Button starboardMouseButton = sf::Mouse::Right;
	};
}
