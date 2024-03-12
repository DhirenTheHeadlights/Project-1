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
		const float tabButtonX = 4.f;
		const float tabButtonY = 4.f;

		const float leftColumnX = 4.f;
		const float leftColumnY = 100.f;

		const float rightColumnX = 400.f;
		const float rightColumnY = 100.f;

		const float rowSpacing = 2.f;
	};
}
