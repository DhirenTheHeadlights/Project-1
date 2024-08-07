#pragma once

/// Pipe Class

#include <SFML/Graphics.hpp>
#include <vector>

namespace DhruvGame {
	class Pipe {
	public:
		Pipe() {}
		
		
		sf::Sprite& getSprite() { return upper; }
		sf::RectangleShape getBB() { return lowerBB; }
		float getDistance() { return distance; }
		void initializePipe();
		void setPosition(sf::Vector2f position);
		void draw(sf::RenderWindow* window, bool debug = false) {
			window->draw(upper);
			window->draw(lower);
			if (!debug) return;
			window->draw(upperBB);
			window->draw(lowerBB);
		}
	private:
		sf::Sprite upper;
		sf::Sprite lower;
		sf::RectangleShape lowerBB;
		sf::RectangleShape upperBB;
		float scaleSizex = 5.f;
		float scaleSizey = 8.f;
		sf::View View;
		float distance;
	};
}