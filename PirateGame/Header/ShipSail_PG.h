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

		void updateSail(const sf::Sprite& shipSprite, const sf::Vector2f& shipDirection, const float maxRotationOffset);
		void moveLeft(const float rotationSpeed) { rotationOffset -= rotationSpeed; }
		void moveRight(const float rotationSpeed) { rotationOffset += rotationSpeed; }
		void moveUp() { /* implement later */ };
		void moveDown() { /* implement later */ };
		void updateSailLeftRightAutomatically(const sf::Vector2f& windDirection, const float shipRotation, const float rotationSpeed);

		sf::Sprite& getSprite() { return sailSprite; }
		sf::Vector2f getOffset() const { return offset; }
		sf::Vector2f getDirectionVector() const { return vm::angleDegreesToVector(sailSprite.getRotation()); }
		float getRotationOffset() const { return rotationOffset; }

		void setOffset(sf::Vector2f offset) { this->offset = offset; }
	private:
		sf::Sprite sailSprite;

		sf::Vector2f offset;
		float rotationOffset = 0.f;
	};
}