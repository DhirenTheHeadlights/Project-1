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
