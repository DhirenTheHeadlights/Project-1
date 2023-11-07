#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(const sf::Vector2f& position, const sf::Vector2f& size, float speed);
    void update(const sf::Time& dt);
    void draw(sf::RenderWindow& window);
    void onCollision(); // Called when the enemy collides with something
    bool isAlive() const;
    void takeDamage(int damage);
    const sf::RectangleShape& getShape() const; // Declaration of getShape


private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float speed;
    int health;
};

#endif // ENEMY_H
