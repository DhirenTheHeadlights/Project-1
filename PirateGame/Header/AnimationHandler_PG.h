#pragma once

/// This class sets up and runs sprite animations.

#include <SFML/Graphics.hpp>

namespace PirateGame {
	class AnimationHandler {
	public:
		AnimationHandler() : numFrames(0), currentFrame(0) {}

		void initialize(const std::string& texturePath, int numFrames, sf::Vector2f frameSize, sf::Time frameTime) {
			if (!texture.loadFromFile(texturePath)) {
				throw std::runtime_error("Failed to load texture");
			}

			this->numFrames = numFrames;
			this->frameSize = frameSize;
			this->frameTime = frameTime;

			texture.setRepeated(true);
			sprite.setTexture(texture);
			sprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
		}

		void update() {
			sf::Time deltaTime = frameClock.restart();
			elapsedTime += deltaTime;

			if (elapsedTime >= frameTime) {
				elapsedTime = sf::Time::Zero;

				if (currentFrame == numFrames - 1) {
					currentFrame = 0;
				}
				else {
					currentFrame++;
				}

				sprite.setTextureRect(sf::IntRect(currentFrame * frameSize.x, 0, frameSize.x, frameSize.y));
			}
		}

		void draw(sf::RenderWindow* window) const {
			window->draw(sprite);
		}

	private:
		sf::Sprite sprite;
		sf::Texture texture;

		int numFrames;
		int currentFrame;

		sf::Clock frameClock;
		sf::Time frameTime;
		sf::Time elapsedTime;
		
		sf::Vector2f frameSize;
	};
}