#pragma once

/// <summary>
/// The base class for all of the game menus.
/// It is intended to be inherited from and extended to create new menus.
/// </summary>

#include <SFML/Graphics.hpp>
#include "Interactable_PG.h"

class Menu {
public:
	Menu(sf::RenderWindow& window, sf::Font& font);
	~Menu() {};

	virtual void setUpMenu();
	virtual void draw(sf::RenderWindow& window);
	virtual void addInteractable();
	virtual void setInteractablePositions() = 0;
	virtual void interactWithMenuItems();

	// Setters for properties
	void setTitleString(std::string titleString) { this->title = titleString; }
	

private:
	// SFML objects
	sf::Font& font;
	sf::RenderWindow& window;

	// Menu Text
	std::string title = "Title not set";
	sf::Text titleText;

	// Menu items
	sf::RectangleShape backgroundRect;
	sf::RectangleShape menu;
	std::vector<std::unique_ptr<Interactable>> interactables;

	// Menu colors
	sf::Color backgroundColor = sf::Color(0, 0, 0, 200);
	sf::Color textColor = sf::Color::White;
	sf::Color interactableColor = sf::Color(0, 0, 0, 200);
	sf::Color interactableTextColor = sf::Color::White;
	sf::Color hoverColor = sf::Color(0, 0, 0, 100);

	// Menu properties
	sf::Vector2f size = sf::Vector2f(1000.0f, 500.0f);
	sf::Vector2f interactableSize = sf::Vector2f(100.0f, 50.0f);
	float textSize = 20.0f;

	// Menu variables
	bool isMenuOpen = false;
	bool redraw = false;
	bool positionUpdated = false;
	bool wasButtonPressedLastFrame = false;
	sf::Time totalPauseDuration;
};
