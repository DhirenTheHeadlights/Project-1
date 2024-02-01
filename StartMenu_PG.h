#pragma once

/// <summary>
/// This is a child menu class for the start menu.
/// </summary>

#include "Menu_PG.h"

class StartMenu : public Menu {
public:
	StartMenu(sf::RenderWindow& window, sf::Font& font, GameStateManager& GSM) : Menu(window, font), GSM(GSM) {};
	~StartMenu() {};

	void setUpMenu() override;
	void setInteractablePositions() override;
	void addInteractables() override;

private:
	// GameStateManager
	GameStateManager& GSM;
};

