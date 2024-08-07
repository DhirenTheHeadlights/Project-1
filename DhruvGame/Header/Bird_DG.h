#pragma once

/// Bird Class


#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

namespace DhruvGame {
	class Bird {
	public:
		Bird() {}

		sf::Sprite& getSprite() { return sprite; }
		sf::RectangleShape& getLowerBB() { return boundingBoxLower; }
		sf::RectangleShape& getMidBB() { return boundingBoxMid; }
		sf::RectangleShape& getUpperBB() { return boundingBoxUpper; }
		float getVelocity() { return velocity; }

		void moveBird();
		void setVelocity(float velocity) { this->velocity = velocity; }
		void setSprite(sf::Sprite sprite) { this->sprite = sprite; }
		void initializeBird();
		void draw(sf::RenderWindow* window, bool debug = false) {
			window->draw(sprite);
			if (!debug) return;
			window->draw(boundingBoxLower);
			window->draw(boundingBoxMid);
			window->draw(boundingBoxUpper);
		}

	private:
		sf::Sprite sprite;
		sf::RectangleShape boundingBoxLower;
		sf::RectangleShape boundingBoxMid;
		sf::RectangleShape boundingBoxUpper;
		float scaleSize = 3.f;
		float gravity = .8f;
		float velocity = 1.f;
		sf::Vector2f lowerOffset = { 0.f, 29.f };
		sf::Vector2f midOffset = { 4.f, 24.f };
		sf::Vector2f upperOffset = { 8.f, 14.f };
		
		

	};
}



//160, 110, 96 -> 366