// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Room.h" // Make sure you're including Room so Player knows about it
#include "Bullet.h"


class Player {
public:
    Player(); // Constructor

    void init(float x, float y, float width, float height); // Initializes the player
    void update(const sf::Time& dt); // Updates the player's actions
    void draw(sf::RenderWindow& window); // Draws the player to the window
    void checkCollisionWithRoom(const Room& room); // This should not be static
    void shoot(const sf::Vector2f& direction); // Shoot a bullet in the given direction
    void updateBullets(const sf::Time& dt, sf::RenderWindow& window); // Handle updating and drawing bullets
    std::vector<Bullet>& getBullets(); // Return a reference to the bullets vector


private:
    sf::RectangleShape shape;
    std::vector<Bullet> bullets;
    sf::Time fireCooldown; // Time until the next shot is allowed
    sf::Time fireRate; // Time between shots
};

#endif // PLAYER_H
