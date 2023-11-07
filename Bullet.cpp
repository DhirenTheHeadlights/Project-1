#include "Bullet.h"

// Bullet class implementation
Bullet::Bullet(const sf::Vector2f& position, const sf::Vector2f& direction, float speed)
    : direction(direction), speed(speed) {
    shape.setPosition(position);
    shape.setSize(sf::Vector2f(2.f, 5.f)); // Small bullet size
    shape.setFillColor(sf::Color::Red); // Bullet color
}

// Update the bullet's speed and position
void Bullet::update(const sf::Time& dt) {
    // Move the bullet in its direction
    shape.move(direction * speed * dt.asSeconds());
}

const sf::RectangleShape& Bullet::getShape() const {
    return shape;
}


// Display the bullet
void Bullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

// Check if the bullet is off-screen
bool Bullet::isOffScreen(const sf::RenderWindow& window) {
    // Check if the bullet is outside the window bounds
    sf::FloatRect bounds = shape.getGlobalBounds();
    return (bounds.top < 0 || bounds.top > window.getSize().y ||
        bounds.left < 0 || bounds.left > window.getSize().x);
}
