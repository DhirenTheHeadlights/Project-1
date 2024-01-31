#include "Menu_PG.h"

Menu::Menu(sf::RenderWindow& window, sf::Font& font) : window(window), font(font) {
	// Set up the background rectangle
	backgroundRect = sf::RectangleShape(sf::Vector2f(window.getSize().x, window.getSize().y));
	backgroundRect.setFillColor(backgroundColor);

	// Set up the menu rectangle
	menu = sf::RectangleShape(size);
	menu.setFillColor(backgroundColor);

	// Set up the title text
	titleText = sf::Text(title, font);
	titleText.setCharacterSize(textSize);
	titleText.setFillColor(textColor);
	titleText.setStyle(sf::Text::Bold);
}

// Add an interactable to the menu
void Menu::addInteractable(std::unique_ptr<Interactable> interactable) {
	// Add the interactable to the list of interactables
	interactables.push_back(interactable);
}

// Set up the menu
void Menu::setUpMenu() {
	// Set the position of the menu to be in the center of the screen
	sf::Vector2f position = sf::Vector2f(window.getSize().x / 2 - size.x / 2, window.getSize().y / 2 - size.y / 2);
	menu.setPosition(position);

	// Set the position of the title
	titleText.setPosition(position.x + 10, position.y + 10);

	// Set the position of the interactables
	setInteractablePositions();
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
}

// Interact with the menu items
void Menu::interactWithMenuItems() {
	// Check if the mouse is hovering over any of the interactables
	for (auto& interactable : interactables) {
		interactable->interact(window);
	}
}