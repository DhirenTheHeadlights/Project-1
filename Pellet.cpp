#include "Pellet.h"

const std::vector<sf::Color> Pellet::colors = {
	sf::Color::Red,
	sf::Color::Green,
	sf::Color::Blue,
	sf::Color::Yellow,
	sf::Color::Cyan,
	sf::Color::Magenta,
	sf::Color::White
};

Pellet::Pellet(float x, float y) {
    shape.setPosition(x, y);
    shape.setRadius(radius);
    int randomIndex = std::rand() % colors.size();
    sf::Color randomColor = colors[randomIndex];
    shape.setFillColor(randomColor);
}

sf::Vector2f Pellet::getPosition() const {
    return shape.getPosition();
}

int Pellet::getRadius() const {
    return radius;
}

void Pellet::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(shape);
    }
}

bool Pellet::deActivate() {
    return active = false;
}

bool Pellet::isActive() const {
    return active;
}
