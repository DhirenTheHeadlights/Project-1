#include "View_PG.h"

using namespace PirateGame;

void View::setUpView() {
	window = GlobalValues::getInstance().getWindow();
}

void View::setCenter(sf::Vector2f position) {
	view.setCenter(position);
	sf::Vector2f size = sf::Vector2f(window->getSize());
	view.setSize(size);
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
		view.move(0.f, -5.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		view.move(-5.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		view.move(0.f, 5.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		view.move(5.f, 0.f);
	}

	window->setView(view);
}