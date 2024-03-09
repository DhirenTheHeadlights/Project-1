#pragma once

/// This is a child menu class for the start menu.

#include "Menu_PG.h"

namespace PirateGame {
	class StartMenu : public Menu {
	public:
		StartMenu() : Menu() {
			buttonSprite.setTexture(interactableTextures.getBlackGrayButton());
		};
		~StartMenu() {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;

	private:

		sf::Sprite buttonSprite;
	};
}