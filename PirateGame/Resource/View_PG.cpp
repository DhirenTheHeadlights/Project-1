#include "View_PG.h"

using namespace PirateGame;

void View::setUpView(const sf::Vector2f& initalPosition, const sf::Vector2f& size) {
	view.setCenter(initalPosition);
	view.setSize(size);
}

void View::updateView(const sf::Vector2f& targetPos, const sf::Vector2f& offset) {
	view.move(offset);
	if (std::fabs(view.getCenter().x - targetPos.x) > 0.1f) {
		view.setCenter(targetPos); // In case the view shifts rapidly
	}
	window->setView(view);
}

// This function uses WASD to move the view around the map, for debugging purposes
void View::updateDebugView(sf::Event event) {
	if (!sizeNotSet) {
		sf::Vector2f size = sf::Vector2f(window->getSize());
		view.setSize(size);
		sizeNotSet = true;
	}

	// Zoom in and out with the scroll wheel
	if (event.type == sf::Event::MouseWheelScrolled) {
		if (event.mouseWheelScroll.delta > 0) {
			view.zoom(0.99f);
		}
		else {
			view.zoom(1.01f);
		}
	}

	// or, zoom in and out with the up and down arrow keys
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		view.zoom(0.99f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		view.zoom(1.01f);
	}

	// Move the view with WASD
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		view.move(0.f, -5.f * window->getView().getSize().y / window->getSize().y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		view.move(-5.f * window->getView().getSize().x / window->getSize().x, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		view.move(0.f, 5.f * window->getView().getSize().y / window->getSize().y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		view.move(5.f * window->getView().getSize().x / window->getSize().x, 0.f);
	}

	window->setView(view);
}

void View::showCoordsOnCursor(const sf::Font& font) const {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	sf::Vector2f worldPosition = window->mapPixelToCoords(mousePosition);
	std::string coords = "Coords: " + std::to_string(static_cast<int>(worldPosition.x)) + ", " + std::to_string(static_cast<int>(worldPosition.y));
	sf::Text coordsText(coords, font, coordsTextSize);
	coordsText.setPosition(worldPosition.x + coordsOffset, worldPosition.y + coordsOffset);
	window->draw(coordsText);
}
