#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Interactable_PG.h"

// This class creates an interactable for a menu to display text only

namespace PirateGame {
	class TextDisplayBox : public Interactable {
	public:
		TextDisplayBox(sf::Font& font) : Interactable() {};
	private:

	};
} 

