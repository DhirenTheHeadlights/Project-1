#include "Menu_PG.h"
#include "Ship_PG.h"
/*
namespace PirateGame {
	class IslandMenu : public Menu {
	public:
		IslandMenu(GameStateManager& GSM) : GSM(GSM) {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void draw() override;

		// Setters
		void setShip(Ship& ship) { this->ship = &ship; };
	private:
		// GameStateManager
		GameStateManager& GSM;

		// Menu items
		float padding = 10.f;

		Ship* ship = nullptr;

		sf::RectangleShape menuBackground;
		std::vector<std::unique_ptr<Button>> buyButtons;
		std::vector<std::unique_ptr<Button>> sellButtons;
		std::vector<std::unique_ptr<TextDisplayBox>> merchandise;



		sf::Text islandName;
	};
}*/