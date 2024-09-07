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
		OptionsMenu(GlobalContext& context) : Menu(context), context(context) {};
		~OptionsMenu() {};

		void setUpMenu() override;
		void draw() override;
		void update() override;

	private:
		// Reference to the global context
		GlobalContext& context;

		// Menu variables
		Tab currentTab = Tab::General;

		std::vector<std::unique_ptr<Interactable>> generalTabInteractables;
		std::vector<std::unique_ptr<Interactable>> graphicsTabInteractables;
		std::vector<std::unique_ptr<Interactable>> audioTabInteractables;
		std::vector<std::unique_ptr<Interactable>> controlsTabInteractables;

		// Menu tabs
		std::vector<Button> tabButtons;

		// Helper functions
		void setInteractablePositions() override;
		void setTabInteractablePositions(std::vector<std::unique_ptr<Interactable>>& tabInteractables) const;
		void addInteractablesToMenu() override;
		void drawTabInteractables(std::vector<std::unique_ptr<Interactable>>& tabInteractables);
		void interactWithMenuItems() override;

		// Overriden function to 
		void addDropDown(sf::Text text, sf::Texture& texture, std::vector<std::unique_ptr<Interactable>>& destination, std::vector<std::pair<std::function<void()>, std::string>> options, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) override {
			std::unique_ptr<DropDown> dropDown = std::make_unique<DropDown>(options);
			dropDown->createInteractable(context.GTH->getOptionsMenuTextures().getDropDown(), text);
			dropDown->setOptionsBoxSprite(context.GTH->getOptionsMenuTextures().getRightInteractable());
			dropDown->setOptionTextColor(sf::Color::Black);
			destination.push_back(std::move(dropDown));
		}

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
