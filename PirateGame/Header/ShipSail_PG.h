#pragma once

/// This is a simple class that encompasses one of the sails on a ship

#include <SFML/Graphics.hpp>

#include "GlobalInputHandler_PG.h"

namespace PirateGame {
	class Sail {
	public:
		Sail(const sf::Texture& texture, sf::Vector2f scaling) : sailSprite(texture) {
			// Set the origin to the center of the sail
			sailSprite.setOrigin(sailSprite.getGlobalBounds().width / 2.f - 1.f, sailSprite.getGlobalBounds().height / 2.f);

			// Set the scale of the sail
			sailSprite.setScale(scaling);
		};
		~Sail() {};

		void updateSail(sf::Sprite& shipSprite);
		void updateSailLeftRight(sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey);
		void updateSailUpDown(sf::Keyboard::Key upKey, sf::Keyboard::Key downKey);

		sf::Sprite& getSprite() { return sailSprite; }
		sf::Vector2f getOffset() const { return offset; }
		float getRotationOffset() const { return rotationOffset; }

		void setOffset(sf::Vector2f offset) { this->offset = offset; }
		void setRotationOffset(float rotationOffset) { this->rotationOffset = rotationOffset; }
	private:
		sf::Sprite sailSprite;

		sf::Vector2f offset;
		float rotationOffset = 0.f;

		const int maxRotationOffset = 45;
	};
}