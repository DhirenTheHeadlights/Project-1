#pragma once

/// This is a child menu class for the start menu.

#include "Menu_PG.h"

namespace PirateGame {
	class StartMenu : public Menu {
	public:
		StartMenu(GameStateManager& GSM) : Menu(), GSM(GSM) {};
		~StartMenu() {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;

	private:
		// GameStateManager
		GameStateManager& GSM;
	};
}