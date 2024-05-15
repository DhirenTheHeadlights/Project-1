#pragma once

/// This class will handle the sails for the ship

#include <SFML/Graphics.hpp>
#include <iostream>

#include "VectorMath.h"

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

		sf::Vector2f getSailDirectionVector() {
			float avgRotation = 0.f;

			// Calculate the average rotation of all sails
			for (auto& sail : sails) {
				avgRotation += sail.getSprite().getRotation();
			}
			avgRotation /= sails.size();

			// Convert average rotation to radians
			const float PI = 3.14159265f;
			float rotationRad = avgRotation * PI / 180.f;

			// Determine the direction vector based on the average rotation
			sf::Vector2f sailDirection(std::sin(rotationRad), -std::cos(rotationRad));

			return sailDirection;
		}
	private:
		std::vector<Sail> sails;

		sf::Sprite& shipSprite;
	};
}