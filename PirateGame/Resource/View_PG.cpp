#include "View_PG.h"

using namespace PirateGame;

View::View(sf::RenderWindow& window) : window(window) {
	view = window.getDefaultView();
}

View::~View() {
}

void View::setCenter(sf::Vector2f position) {
	view.setCenter(position);
	window.setView(view);
}
