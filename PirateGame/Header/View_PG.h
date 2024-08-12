#pragma once

/// This class is used to represent the view in the game.
/// The view is the part of the game that the player can see.

#include <SFML/Graphics.hpp>

namespace PirateGame {
	class View {
	public:
		View() {};
		View(sf::RenderWindow* window) : window(window) {};
		~View() {};

		void setCenter(sf::Vector2f position);
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

