#pragma once

/// This class will handle the sails for the ship

#include <SFML/Graphics.hpp>
#include <iostream>

#include "VectorMath.h"
#include "GlobalInputHandler_PG.h"

#include "ShipSail_PG.h"

namespace PirateGame {
	class ShipSailHandler {
	public:
		ShipSailHandler() {};

		void loadSailPositions(const std::vector<sf::Texture>& sailTextures, const sf::Image& shipImage, sf::Vector2f scaling);

		void update(const sf::Sprite& shipSprite, const sf::Vector2f& shipDirection, const float maxAngle);

		void moveSailsUp(sf::Keyboard::Key key) {
			for (auto& sail : sails) {
				sail.moveUp();
			}
		}
		void moveSailsDown(sf::Keyboard::Key key) {
			for (auto& sail : sails) {
				sail.moveDown();
			}
		}
		void moveSailsLeft(sf::Keyboard::Key key, const float rotationSpeed) {
			for (auto& sail : sails) {
				sail.moveLeft(rotationSpeed);
			}
		}
		void moveSailsRight(sf::Keyboard::Key key, const float rotationSpeed) {
			for (auto& sail : sails) {
				sail.moveRight(rotationSpeed);
			}
		}

		void moveSailLeftRightAutomatically(const sf::Vector2f& windDirection, const float shipRotation, const float rotationSpeed);
		void draw(sf::RenderWindow* window);

		sf::Vector2f getAverageSailDirection() {
			float avgRotation = 0.f;

			// Calculate the average rotation of all sails
			for (auto& sail : sails) {
				avgRotation += sail.getSprite().getRotation();
			}
			avgRotation /= sails.size();

			// Convert average rotation to a vector
			return vm::angleDegreesToVector(avgRotation);
		}
		std::vector<Sail>& getSails() { return sails; }
	private:
		std::vector<Sail> sails;
	};
}