#pragma once

/// This class will handle the sails for the ship

#include <SFML/Graphics.hpp>
#include <iostream>

#include "GlobalValues_PG.h"
#include "GlobalTextureHandler_PG.h"
#include "GlobalInputHandler_PG.h"
#include "ShipEnums_PG.h"
#include "ShipSail_PG.h"

namespace PirateGame {
	class ShipSailHandler {
	public:
		ShipSailHandler(sf::Sprite& shipSprite) : shipSprite(shipSprite) {};
		~ShipSailHandler() {};

		void loadSailPositions(ShipClass shipClass, sf::Vector2f scaleing);

		void update(sf::Sprite& shipSprite);
		void moveSailsUpAndDown(sf::Keyboard::Key upKey, sf::Keyboard::Key downKey);
		void moveSailLeftRight(sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey);
		void draw();
	private:
		std::vector<Sail> sails;

		sf::Sprite& shipSprite;
	};
}