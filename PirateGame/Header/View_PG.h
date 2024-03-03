#pragma once

/// <summary>
/// This class is used to represent the view in the game.
/// The view is the part of the game that the player can see.
/// </summary>

#include <SFML/Graphics.hpp>
#include "GlobalValues_PG.h"

namespace PirateGame {
	class View {
	public:
		View() {};
		~View() {};

		void setUpView();
		void setCenter(sf::Vector2f position);

		sf::View& getView() { return view;  }
	private:
		sf::View view;
		sf::RenderWindow* window = nullptr;
	};
}

