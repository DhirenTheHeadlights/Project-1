#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& position, const sf::Vector2f& size, float speed)
    : velocity(0, speed), health(100) {
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color::Red); // For example, enemies could be red

    // Set the origin to the center of the shape
    shape.setOrigin(size.x / 2.f, size.y / 2.f);

    // Set the velocity to the given speed
    velocity.y = speed;

    // Set the health to 100
    health = 100;

    // Set the speed to the given speed
    this->speed = speed;

}

void Enemy::update(const sf::Time& dt) {
    // Simple downward movement for now
    shape.move(0, velocity.y * dt.asSeconds());
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

const sf::RectangleShape& Enemy::getShape() const {
    return shape;
}

void Enemy::onCollision() {
    // Implement what happens when an enemy collides with something
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        // Handle enemy death, like setting it to not alive or removing it from the game.
    }
}
