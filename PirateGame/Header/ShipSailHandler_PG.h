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
            sf::Vector2f avgPosition(0.f, 0.f);
            float avgRotation = 0.f;

            // Calculate the average position and rotation of all sails
            for (auto& sail : sails) {
                avgPosition += sail.getSprite().getPosition();
                avgRotation += sail.getSprite().getRotation();
            }
            avgPosition /= static_cast<float>(sails.size());
            avgRotation /= sails.size();

            // Compute the direction vector from the ship's position to the average sail position
            sf::Vector2f sailDirection = avgPosition - shipSprite.getPosition();

            // Normalize the direction vector
            sailDirection = vm::normalize(sailDirection);

            // Apply the average rotation to the direction vector
            const float PI = 3.14159265f;
            float rotationRad = avgRotation * PI / 180.f;
            sailDirection = sf::Vector2f(
                sailDirection.x * cos(rotationRad) - sailDirection.y * sin(rotationRad),
                sailDirection.x * sin(rotationRad) + sailDirection.y * cos(rotationRad)
            );

            return sailDirection;
		}
	private:
		std::vector<Sail> sails;

		sf::Sprite& shipSprite;
	};
}