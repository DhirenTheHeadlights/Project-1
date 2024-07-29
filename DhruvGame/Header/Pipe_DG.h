#pragma once

/// Pipe Class

#include <SFML/Graphics.hpp>
#include <vector>

namespace DhruvGame {
	class Pipe {
	public:
		Pipe() {}
		
		
		sf::Sprite& getSprite() { return sprite; }
		sf::RectangleShape getBB() { return boundingBox; }
		void setBoundingPosition();
		void initializePipe();
	private:
		sf::Sprite sprite;
		sf::RectangleShape boundingBox;
		float scaleSizex = 6.f;
		float scaleSizey = 7.f;
	};
}