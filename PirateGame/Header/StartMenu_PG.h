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
		void setUpInteractable(std::function<void()> function, sf::Text name, sf::Vector2f scale);
	private:
		// Menu parameters
		sf::Vector2f interactableScale = { 1.0f, 1.0f };
	};
}