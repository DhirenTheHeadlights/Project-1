#pragma once

/// This is a child menu class for the options menu.

#include "Menu_PG.h"

// Enum class for different tabs
enum class Tab {
	General,
	Graphics,
	Audio,
	Controls
};

namespace PirateGame {
	class OptionsMenu : public Menu {
	public:
		OptionsMenu() : Menu() {};
		~OptionsMenu() {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void setTabInteractablePositions(std::vector<std::unique_ptr<Interactable>>& tabInteractables);
		void addInteractablesToMenu() override;
		void draw() override;
		void drawTabInteractables(std::vector<std::unique_ptr<Interactable>>& tabInteractables);
		void interactWithMenuItems() override;

		// Add interactables to the tabs
		void addGeneralTabInteractables();
		void addGraphicsTabInteractables();
		void addAudioTabInteractables();
		void addControlsTabInteractables();

	private:
		// Menu variables
		Tab currentTab = Tab::General;
		std::vector<std::unique_ptr<Interactable>> generalTabInteractables;
		std::vector<std::unique_ptr<Interactable>> graphicsTabInteractables;
		std::vector<std::unique_ptr<Interactable>> audioTabInteractables;
		std::vector<std::unique_ptr<Interactable>> controlsTabInteractables;

		// Menu items
		sf::RectangleShape tabBar;
		sf::Vector2f tabSize = sf::Vector2f(200.0f, 50.0f);

		// Menu tabs
		std::vector<std::unique_ptr<Button>> tabButtons;
	};
}
