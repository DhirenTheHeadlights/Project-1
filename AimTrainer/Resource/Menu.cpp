#include "Menu.h"

Menu::Menu(sf::RenderWindow& window, sf::Font font_in) : window(window), font(font_in) {

	// Set the size of the menu background
	backgroundRect = sf::RectangleShape(sf::Vector2f(window.getSize().x, window.getSize().y));
	backgroundRect.setFillColor(backgroundColor);

	// Set the size and color of the menu
	menu = sf::RectangleShape(sf::Vector2f(length, width)); 
	menu.setFillColor(backgroundColor);

	// Set the title properties
	title = sf::Text("Title Not Set" , font);
	title.setCharacterSize(50.0f);
	title.setFillColor(textColor);
	title.setStyle(sf::Text::Bold);
}

///////////////////////////////////////////////////////////////////
// Constructor Functions for the Menu /////////////////////////////
///////////////////////////////////////////////////////////////////

void Menu::setTotalPauseTime(sf::Time time) {
	totalPauseDuration = time;
}

void Menu::resetRedrawFlag() {
	redraw = false;
}

void Menu::setTitleString(std::string titleString) {
	title.setString(titleString);
}

void Menu::setMenuPosition(sf::Vector2f position) {
	menu.setPosition(position);
}

void Menu::addInteractable(std::string name, Interactable::Type type, std::optional<std::function<void(float value)>> func) {
	// Create a new interactable and add it to the vector if there is room
	Interactable* interactable = new Interactable(func, type);
	interactable->createInteractable(name, buttonLen, buttonWid, textSize);
	interactables.push_back(interactable);
}

void Menu::setButtonSize(sf::Vector2f size) {
	buttonLen = size.x;
	buttonWid = size.y;
}

void Menu::setTextSize(float size) {
	textSize = size;
}
///////////////////////////////////////////////////////////////////
// Functions for the Menu /////////////////////////////////////////
///////////////////////////////////////////////////////////////////

void Menu::openMenu(sf::Event& eventFromMain, Crosshair& xhair, sf::Clock& mainClock) {
	if (isMenuOpen) {
		drawMenu(eventFromMain);
		interactWithMenuItems();
		xhair.draw(false);
	}
}

// Toggle the menu open or closed

void Menu::toggleMenu(sf::Clock& mainClock, Crosshair& xhair) {
	isMenuOpen = !isMenuOpen; // Toggle the state of the menu

	// Handle the pausing of the main clock when the menu is open
	if (isMenuOpen) {
		xhair.draw(false); // Optionally update the crosshair or other UI elements
		pauseTimeStart = mainClock.getElapsedTime(); // Start measuring pause duration
	}
	else {
		totalPauseDuration += mainClock.getElapsedTime() - pauseTimeStart;
		pauseTimeStart = sf::Time::Zero; // Reset pause start time
	}

	redraw = true; // Indicate that the menu needs to be redrawn
}


// Handle the pausing of the main clock when the menu is open

void Menu::handlePauseTime(sf::Clock& mainClock, Crosshair& xhair) {
	if (isMenuOpen) {
		xhair.draw(false);
		pauseTimeStart = mainClock.getElapsedTime();
	}
	else {
		totalPauseDuration += mainClock.getElapsedTime() - pauseTimeStart;
		pauseTimeStart = sf::Time::Zero;
	}
}

// Draw the menu to the screen and set the positions of the interactables

void Menu::drawMenu(sf::Event event) {
	if (!isMenuOpen) return;

	// Set the size and position of the menu
	float inputBoxOffset = 10.0f; // Offset between the input box and the slider
	float menuWidth = buttonLen + 50.0f + inputBoxOffset + buttonLen / 10; // This adds the input box and offset to the menu width
	float menuHeight = calculateMenuHeight();
	menu.setSize(sf::Vector2f(menuWidth, menuHeight));
	menu.setPosition(window.getSize().x / 2 - menuWidth / 2, window.getSize().y / 2 - menuHeight / 2);
	menu.setOutlineColor(sf::Color::Black);
	menu.setOutlineThickness(5.0f);

	// Draw the menu background
	window.draw(backgroundRect);
	window.draw(menu);

	// Set the positions of the interactables and draw them
	setInteractablePositions();
	for (auto& interactable : interactables) {
		interactable->draw(window, event);
	}

	// Set and draw the title
	float titleSpacing = 50.0f; // Spacing between title and menu
	title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, menu.getPosition().y - title.getGlobalBounds().height - titleSpacing);
	window.draw(title);
}

float Menu::calculateMenuHeight() {
	// Calculate total height required for all interactables with padding and spacing
	float spacing = 20.0f; // Spacing between interactables
	float interactableHeight = buttonWid; // Assuming all interactables have the same height
	float totalHeight = interactables.size() * (interactableHeight + spacing) + spacing; // Add the height of each interactable and spacing
	return totalHeight;
}


void Menu::setInteractablePositions() {
	const float spacing = 20.0f; // Spacing between interactables
	float startY = menu.getPosition().y + spacing; // Start Y position below title with padding

	for (size_t i = 0; i < interactables.size(); i++) {
		float x = window.getSize().x / 2 - interactables[i]->getSize().x / 2; // Centered X position
		float y = startY + i * (buttonWid + spacing); // Stacked Y positions for interactables
		interactables[i]->setPos(sf::Vector2f(x, y));
	}
}


// Interact with the menu items (activate them)

sf::Clock Menu::buttonPressClock;  // Initialize the static member

void Menu::interactWithMenuItems() {
	for (auto& interactable : interactables) {
		interactable->interact(window, buttonPressClock);
	}
}

///////////////////////////////////////////////////////////////////
// Set the color of all of the menu elements //////////////////////
///////////////////////////////////////////////////////////////////

void Menu::setBackgroundColor(sf::Color color) {
	backgroundColor = color;
	menu.setFillColor(color);
	backgroundRect.setFillColor(color);
}

void Menu::setTextColor(sf::Color color) {
	textColor = color;
	title.setFillColor(color);
}

void Menu::setButtonColor(sf::Color color) {
	buttonColor = color;
	updateInteractableColors();
}

void Menu::setSliderColor(sf::Color color) {
	sliderColor = color;
	updateInteractableColors();
}

void Menu::setSliderComplementColor(sf::Color color) {
	sliderColorComplement = color;
	updateInteractableColors();
}

void Menu::setHoverColor(sf::Color color) {
	hoverColor = color;
	updateInteractableColors();
}

void Menu::updateInteractableColors() {
	for (auto& interactable : interactables) {
		interactable->setTextColor(textColor);
		interactable->setButtonColor(buttonColor);
		interactable->setSliderColor(sliderColor);
		interactable->setSliderComplementColor(sliderColorComplement);
		interactable->setHoverColor(hoverColor);
	}
}

///////////////////////////////////////////////////////////////////
// Getter Functions for the Menu //////////////////////////////////
///////////////////////////////////////////////////////////////////

sf::Time Menu::getTotalTimeInMenu() const {
	return totalPauseDuration;
}

bool Menu::needsRedraw() const {
	return redraw;
}

bool Menu::getIsMenuOpen() const {
	return isMenuOpen;
}

std::string Menu::getInputStringForInteractableI(int i) const {
	return interactables[i]->getInputString();
}

Interactable* Menu::getInteractableI(int i) const {
	return interactables[i];
}