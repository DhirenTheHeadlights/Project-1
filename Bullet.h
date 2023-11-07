#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(const sf::Vector2f& position, const sf::Vector2f& direction, float speed);
    void update(const sf::Time& dt); // Update the bullet's speed and position
    void draw(sf::RenderWindow& window); // Display the bullet
    bool isOffScreen(const sf::RenderWindow& window); // Check if the bullet is off-screen
    const sf::RectangleShape& getShape() const;
private:
    sf::RectangleShape shape; 
    sf::Vector2f direction;
    float speed;
};

#endif // BULLET_H
