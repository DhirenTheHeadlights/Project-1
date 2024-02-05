#include "Menu_PG.h"

using namespace PirateGame;

// Add an interactable to the menu
void Menu::addInteractableToMenu(std::unique_ptr<Interactable> interactable) {
	// Add the interactable to the list of interactables
	interactables.push_back(std::move(interactable));
}

// Draw the menu
void Menu::draw(sf::RenderWindow& window) {
	// Draw the menu items
	window.draw(backgroundRect);
	window.draw(menu);
	window.draw(titleText);

	// Draw the interactables
	for (auto& interactable : interactables) {
		interactable->draw(window);
	}

	// Interact with the menu items
	interactWithMenuItems();
}

// Interact with the menu items
void Menu::interactWithMenuItems() {
	for (auto& interactable : interactables) {
		interactable->interact(window);
	}
}