#pragma once

/// Options menu textures

#include "TextureHandler_PG.h"

namespace PirateGame {
	class OptionsMenuTextureHandler : public TextureHandler {
	public:
		OptionsMenuTextureHandler() {
			// Load all of the textures
			background.loadFromFile("PirateGameSprites/options_menu_background.png");
			leftInteractable.loadFromFile("PirateGameSprites/options_menu_button_gray_left.png");
			dropDown.loadFromFile("PirateGameSprites/options_menu_drop_down_right_gray.png");
			rightInteractable.loadFromFile("PirateGameSprites/options_menu_right_interactable.png");
			tab.loadFromFile("PirateGameSprites/options_menu_tab.png");
		}
		~OptionsMenuTextureHandler() {}

		// Getters
		sf::Texture& getBackground() { return background; }
		sf::Texture& getLeftInteractable() { return leftInteractable; }
		sf::Texture& getDropDown() { return dropDown; }
		sf::Texture& getRightInteractable() { return rightInteractable; }
		sf::Texture& getTab() { return tab; }

	private:
		// Textures
		sf::Texture background;
		sf::Texture leftInteractable;
		sf::Texture dropDown;
		sf::Texture rightInteractable;
		sf::Texture tab;
	};
}