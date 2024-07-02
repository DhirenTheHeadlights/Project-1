#include "Menu_PG.h"

using namespace PirateGame;

// Draw the menu
void Menu::draw() {
	// Draw the menu items
	window->draw(menu);
	window->draw(titleText);

	// Interact with the menu items
	interactWithMenuItems();
}