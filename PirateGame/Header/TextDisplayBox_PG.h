#pragma once
#include <SFML/Graphics.hpp>

#include "Interactable_PG.h"

// This class creates an interactable for a menu to display text only

namespace PirateGame {
	class TextDisplayBox : public Interactable {
	public:
		TextDisplayBox() {};

		// Have to define but it doesn't do anything
		void interact(sf::RenderWindow* window, GlobalSoundManager* GSM) override {};
		void customInteractableSetUp() override {};
	};
} 

