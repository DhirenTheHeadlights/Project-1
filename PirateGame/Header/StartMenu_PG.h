#pragma once

/// This is a child menu class for the start menu.

#include "Menu_PG.h"

namespace PirateGame {
	class StartMenu : public Menu {
	public:
		StartMenu() : Menu() {};
		~StartMenu() {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void interactWithMenuItems() override;
		void draw() override;
		void update() override;

	private:
		// Menu parameters
		sf::Vector2f interactableScale = sf::Vector2f(5.f, 5.f);

		std::vector<Button> buttons;
	};
}