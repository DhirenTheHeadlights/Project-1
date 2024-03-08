#pragma once

/// Interactable textures for the game.

#include "TextureHandler_PG.h"

namespace PirateGame {
	class InteractableTextureHandler : public TextureHandler {
	public:
		InteractableTextureHandler() {
			BlackGrayButton.loadFromFile("PirateGameSprites/Interactable_basic_gray_black.png");
		}
		~InteractableTextureHandler() {};

		// Getters
		sf::Texture& getBlackGrayButton() { return BlackGrayButton; }
	private:
		sf::Texture BlackGrayButton;
	};
}