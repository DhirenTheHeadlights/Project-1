#include "Player.h"
#include "Room.h"
#include <iostream>

// Player class implementation

// Constructor
Player::Player() {
    // Initialize with default values
    this->shape.setSize(sf::Vector2f(50.f, 50.f)); // Default size
    this->shape.setFillColor(sf::Color::Green);    // Default color
    fireRate = sf::seconds(0.2f); // For example, the player can shoot every 0.2 seconds
    fireCooldown = sf::Time::Zero; // The player can shoot immediately at the start
}

// Initialization function
void Player::init(float x, float y, float width, float height) {
    this->shape.setPosition(x, y);
    this->shape.setSize(sf::Vector2f(width, height));
}

// Return a reference to the bullets vector
std::vector<Bullet>& Player::getBullets() {
    return bullets;
}

// Update function for the player
void Player::update(const sf::Time& dt) {
 
    float moveSpeed = 200.f; // Speed in units per second

    // Check for movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        this->shape.move(0, -moveSpeed * dt.asSeconds());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        this->shape.move(0, moveSpeed * dt.asSeconds());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        this->shape.move(-moveSpeed * dt.asSeconds(), 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->shape.move(moveSpeed * dt.asSeconds(), 0);
    }

    // Check for shooting
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        shoot(sf::Vector2f(-1.f, 0.f));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        shoot(sf::Vector2f(1.f, 0.f));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        shoot(sf::Vector2f(0.f, -1.f));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        shoot(sf::Vector2f(0.f, 1.f));
    }

    // Update the fire cooldown
    if (fireCooldown > sf::Time::Zero) {
        fireCooldown -= dt;
    }
}

// Render function for the player
void Player::draw(sf::RenderWindow& window) {
    window.draw(this->shape);
}

void Player::checkCollisionWithRoom(const Room& room) {
    // Get the bounds of the player and the room
    sf::FloatRect playerBounds = this->shape.getGlobalBounds();
    sf::FloatRect roomBounds = room.getBounds();

    // Check for player collision with the room
    if (playerBounds.left < roomBounds.left) {
        this->shape.setPosition(roomBounds.left, this->shape.getPosition().y);
    }
    if (playerBounds.left + playerBounds.width > roomBounds.left + roomBounds.width) {
        this->shape.setPosition(roomBounds.left + roomBounds.width - playerBounds.width, this->shape.getPosition().y);
    }
    if (playerBounds.top < roomBounds.top) {
        this->shape.setPosition(this->shape.getPosition().x, roomBounds.top);
    }
    if (playerBounds.top + playerBounds.height > roomBounds.top + roomBounds.height) {
        this->shape.setPosition(this->shape.getPosition().x, roomBounds.top + roomBounds.height - playerBounds.height);
    }
}

// Shoot a bullet in the given direction
void Player::shoot(const sf::Vector2f& direction) {
    // Check if the player can shoot
    if (fireCooldown <= sf::Time::Zero) {

        // Get the center of the player
        sf::Vector2f position = shape.getPosition() + sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2);
        
        // Create a new bullet and add it to the vector
        float speed = 500.0f; // example speed in units per second

        // Create a new bullet and add it to the vector
        Bullet newBullet(position, direction, speed);
        bullets.push_back(newBullet);
        fireCooldown = fireRate; // Reset the cooldown
    }
}


// Update and draw bullets
void Player::updateBullets(const sf::Time& dt, sf::RenderWindow& window) {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->update(dt);
        if (it->isOffScreen(window)) {
            it = bullets.erase(it); // Remove off-screen bullets
        }
        else {
            ++it;
        }
    }
    for (auto& bullet : bullets) {
        bullet.draw(window);
    }
}