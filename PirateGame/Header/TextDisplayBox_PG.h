#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Interactable_PG.h"

// This class creates an interactable for a menu to display text only

namespace PirateGame {
	class TextDisplayBox : public Interactable {
	public:
		TextDisplayBox() : Interactable() {};
		~TextDisplayBox() {};

		// Have to define but it doesn't do anything
		void interact() override {};
		void customInteractableSetUp() override {};
	private:

	};
} 

