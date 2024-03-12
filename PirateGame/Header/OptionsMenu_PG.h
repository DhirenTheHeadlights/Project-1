#pragma once

/// This is a child menu class for the options menu.

#include "Menu_PG.h"

// Enum class for different tabs
enum class Tab {
	General,
	Graphics,
	Audio,
	Controls,
	Extra
};

namespace PirateGame {
	class OptionsMenu : public Menu {
	public:
		OptionsMenu() : Menu() {};
		~OptionsMenu() {};

		void setUpMenu() override;
		void draw() override;
	private:
		// Menu variables
		Tab currentTab = Tab::General;
		std::vector<std::unique_ptr<Interactable>> generalTabInteractables;
		std::vector<std::unique_ptr<Interactable>> graphicsTabInteractables;
		std::vector<std::unique_ptr<Interactable>> audioTabInteractables;
		std::vector<std::unique_ptr<Interactable>> controlsTabInteractables;

		// Menu tabs
		std::vector<std::unique_ptr<Button>> tabButtons;

		// Helper functions
		void setInteractablePositions() override;
		void setTabInteractablePositions(std::vector<std::unique_ptr<Interactable>>& tabInteractables);
		void addInteractablesToMenu() override;
		void drawTabInteractables(std::vector<std::unique_ptr<Interactable>>& tabInteractables);
		void interactWithMenuItems() override;

		void addTabInteractable(std::function<void()> function, std::string name);
		void addDropDownInteractable(std::vector<std::pair<std::function<void()>, std::string>> options, std::string name, std::vector<std::unique_ptr<Interactable>>& tabInteractables);
		void addSliderInteractable(std::function<void(float value)> function, std::string name, std::vector<std::unique_ptr<Interactable>>& tabInteractables);
		void addButtonInteractable(std::function<void()> function, std::string name, std::vector<std::unique_ptr<Interactable>>& tabInteractables);

		// Add interactables to the tabs
		void addGeneralTabInteractables();
		void addGraphicsTabInteractables();
		void addAudioTabInteractables();
		void addControlsTabInteractables();

		// Very specifc values for setting the positions of the Interactables
		const sf::Vector2f tabButtonPosition = sf::Vector2f(4.f, 2.f);
		const sf::Vector2f leftColumnPosition = sf::Vector2f(4.f, 100.f);
		const sf::Vector2f rightColumnPosition = sf::Vector2f(400.f, 100.f);
		const sf::Vector2f leftBounds = sf::Vector2f(400.f, 100.f);
		const float rowSpacing = 2.f;
	};
}
