#include "Pellet.h"
#include "World.h"
#include "Globals.h"

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
    window.draw(shape);
}

void Pellet::RemovePellet(Pellet pellet) {
    sf::CircleShape shape;
    shape.setPosition(pellet.getPosition().x, pellet.getPosition().y);
    shape.setFillColor(sf::Color::Black);
    window.draw(shape);
}
