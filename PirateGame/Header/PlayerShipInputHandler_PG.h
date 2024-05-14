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
		void handleSailChange() override;

		std::string  getFireKeyString() const { return GlobalValues::getInstance().keyToString(fireKey); }
		std::string  getManualAimKeyString() const { return GlobalValues::getInstance().keyToString(manualAimKey); }
		std::string  getPortMouseButtonString() const { return GlobalValues::getInstance().buttonToString(portMouseButton); }
		std::string  getStarboardMouseButtonString() const { return GlobalValues::getInstance().buttonToString(starboardMouseButton); }
		std::string  getAnchorDropKeyString() const { return GlobalValues::getInstance().keyToString(anchorDropKey); }
	private:
		// Keys
		sf::Keyboard::Key fireKey = sf::Keyboard::Space;
		sf::Keyboard::Key manualAimKey = sf::Keyboard::E;

		sf::Mouse::Button portMouseButton = sf::Mouse::Left;
		sf::Mouse::Button starboardMouseButton = sf::Mouse::Right;

		sf::Keyboard::Key anchorDropKey = sf::Keyboard::Tab;

		sf::Keyboard::Key sailUpKey = sf::Keyboard::I;
		sf::Keyboard::Key sailDownKey = sf::Keyboard::K;
		sf::Keyboard::Key sailLeftKey = sf::Keyboard::A;
		sf::Keyboard::Key sailRightKey = sf::Keyboard::D;
	};
}
