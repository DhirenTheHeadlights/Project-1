#pragma once

/// This is a simple class that encompasses one of the sails on a ship

#include <SFML/Graphics.hpp>

#include "VectorMath.h"

namespace PirateGame {
	class Sail {
	public:
		Sail(const sf::Texture& texture, const sf::Vector2f scaling) : sailSprite(texture) {
			// Set the origin to the center of the sail
			sailSprite.setOrigin(sailSprite.getGlobalBounds().width / 2.f - 1.f, sailSprite.getGlobalBounds().height / 2.f);

			// Set the scale of the sail
			sailSprite.setScale(scaling);
		};

		void updateSail(const sf::Sprite& shipSprite, const sf::Vector2f shipDirection);
		void moveLeft() { rotationOffset -= rotationSpeed; }
		void moveRight() { rotationOffset += rotationSpeed; }
		void moveUp() { /* implement later */ };
		void moveDown() { /* implement later */ };
		void updateSailLeftRightAutomatically(const sf::Vector2f& windDirection, const sf::Vector2f& shipDirection);

		sf::Sprite& getSprite() { return sailSprite; }
		sf::Vector2f getOffset() const { return offset; }
		sf::Vector2f getDirectionVector() const {
			const float pi = 3.14159265f;
			float rotationRad = sailSprite.getRotation() * pi / 180.f;
			return sf::Vector2f(std::sin(rotationRad), -std::cos(rotationRad));
		}
		float getRotationOffset() const { return rotationOffset; }

		void setOffset(sf::Vector2f offset) { this->offset = offset; }
	private:
		float calculateAngleRelativeToShip(const sf::Vector2f& shipDirection) const;

		sf::Sprite sailSprite;

		sf::Vector2f offset;
		float rotationOffset = 0.f;

		const float maxRotationOffset = 45.f;
		const float rotationSpeed = 0.1f;
	};
}