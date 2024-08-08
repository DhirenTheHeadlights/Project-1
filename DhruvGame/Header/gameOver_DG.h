#pragma once

/// Game Over Class

#include <SFML/Graphics.hpp>


//1,1; 1,12; 1,22

namespace DhruvGame {
	class gameOver {
	public:
		gameOver() {}
		sf::Vector2f getCenter(sf::RenderWindow* window);
		sf::Sprite& getSprite() { return sprite; }
		void initializegameOver();
		sf::RectangleShape& getRestartButton() { return restartButton; }
		sf::RectangleShape& getQuitButton() { return quitButton; }
		void draw(sf::RenderWindow* window, bool debug = false) {
			window->draw(sprite);
			if (!debug) return;
			window->draw(restartButton);
			window->draw(quitButton);
		}
	private:
		sf::Sprite sprite;
		sf::Texture texture;
		sf::RectangleShape restartButton;
		sf::RectangleShape quitButton;
		sf::Vector2f scaleSize = { 10.f, 10.f };
		sf::Vector2f gameOverPosition = { 500, 500 };

	};
}