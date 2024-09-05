#pragma once

/// This class is used to represent the view in the game.
/// The view is the part of the game that the player can see.

#include <SFML/Graphics.hpp>

namespace PirateGame {
	class View {
	public:
		View() {};
		View(sf::RenderWindow* window) : window(window) {};

		void setUpView(const sf::Vector2f& initialPosition, const sf::Vector2f& size);
		void updateView(const sf::Vector2f& targetPos, const sf::Vector2f& offset);
		void updateDebugView(sf::Event event);
		void showCoordsOnCursor(const sf::Font& font);

		sf::View& getView() { return view;  }
	private:
		sf::View view;
		sf::RenderWindow* window = nullptr;

		int scale = 2;
		int coordsTextSize = 20;

		float coordsOffset = 20.f;

		bool sizeNotSet = false;
	};
}

