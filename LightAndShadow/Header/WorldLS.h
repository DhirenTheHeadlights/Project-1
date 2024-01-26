#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

class LightAndShadow {
public:
    LightAndShadow(sf::RenderWindow& window);
    void run(sf::Event event);
private:
    void initializeGameEntities();
    void update();
    void render();
    void generateObstacles();
    std::vector<sf::Vector2f> calculateShadow(const sf::RectangleShape& box, const sf::Vector2f& lightPos);
    sf::ConvexShape createShadowShape(const std::vector<sf::Vector2f>& points);

    sf::RenderWindow& window;
    sf::RectangleShape background;
    sf::CircleShape lightEmitter;
    sf::ConvexShape lightShape;
    sf::ConvexShape player;
    sf::Clock mainClock;
    std::vector<sf::RectangleShape> obstacles;

    float obstacleSize = 100;
    float playerSize = 50;
    float lightEmitterSize = 30;
};
