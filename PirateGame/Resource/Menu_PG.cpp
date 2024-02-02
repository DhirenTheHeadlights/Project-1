#include "Menu_PG.h"

using namespace PirateGame;

Menu::Menu(sf::RenderWindow& window, sf::Font& font) : window(window), font(font) {
	// Set up the background rectangle
	backgroundRect = sf::RectangleShape(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	backgroundRect.setFillColor(backgroundColor);

	// Set up the menu rectangle
	menu = sf::RectangleShape(size);
	menu.setFillColor(backgroundColor);

	// Set up the title text
	titleText = sf::Text(title, font);
	titleText.setCharacterSize(static_cast<unsigned int>(textSize));
	titleText.setFillColor(textColor);
	titleText.setStyle(sf::Text::Bold);
}

// Add an interactable to the menu
void Menu::addInteractableToMenu(std::unique_ptr<Interactable> interactable) {
	// Add the interactable to the list of interactables
	interactables.push_back(std::move(interactable));
}

// Set up the menu
void Menu::setUpMenu() {
	// Set the position of the menu to be in the center of the screen
	sf::Vector2f position = sf::Vector2f(window.getSize().x / 2 - size.x / 2, window.getSize().y / 2 - size.y / 2);
	menu.setPosition(position);

	// Set the position of the title
	titleText.setPosition(position.x + 10, position.y + 10);

	// Set the position of the interactables
	addInteractables();
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

	// Interact with the menu items
	interactWithMenuItems();
}

// Interact with the menu items
void Menu::interactWithMenuItems() {
	for (auto& interactable : interactables) {
		interactable->interact(window);
	}
}