#include "Pellet.h"
#include "World.h"
#include "Globals.h"

Pellet::Pellet(float x, float y) {
    shape.setPosition(x, y);
    shape.setRadius(5);
    int randomIndex = std::rand() % colors.size(); 
    sf::Color randomColor = colors[randomIndex]; 
    shape.setFillColor(randomColor);
}

void Pellet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}