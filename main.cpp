#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

// Constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const float MAX_ACCELERATION = 10.0f;
const float BASE_SPEED = 0.01f;

class Circle {
public:
    Circle(float radius) : shape(radius), circlesize(radius) {
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Blue);
    }

    void move(float dx, float dy) {
        x += dx;
        y += dy;
        // Boundary checks
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > SCREEN_WIDTH - 2 * circlesize) x = SCREEN_WIDTH - 2 * circlesize;
        if (y > SCREEN_HEIGHT - 2 * circlesize) y = SCREEN_HEIGHT - 2 * circlesize;
        shape.setPosition(x, y);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    float getCirclesize() const {
        return circlesize;
    }

private:
    sf::CircleShape shape;
    float x{ 0 };
    float y{ 0 };
    float circlesize;
};

int main() { 
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "RANGER WINDOW");
    Circle circle(30.f);
    sf::Clock mainTime;
    sf::Clock speedClock;
    sf::Clock deltaClock;  // Clock to measure time since last frame
    double acceleration = 0.0;
    double speed = BASE_SPEED;
    sf::Font font;
    sf::Text text;

    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();  // Time since last frame in seconds
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (speedClock.getElapsedTime().asSeconds() >= 2) { // Update acceleration and speed
            acceleration = 0.0;
        }
        else {
            acceleration += 0.0001;  // Smaller increment value
            if (acceleration > MAX_ACCELERATION) acceleration = MAX_ACCELERATION;
        }
        speed = BASE_SPEED + acceleration;

        // Movement
        float moveSpeed = speed * deltaTime * 2000;  // Multiply by deltaTime and a scaling factor for control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && circle.getCirclesize() > 0) circle.move(0, -moveSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && circle.getCirclesize() < SCREEN_HEIGHT) circle.move(0, moveSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && circle.getCirclesize() > 0) circle.move(-moveSpeed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && circle.getCirclesize() < SCREEN_WIDTH) circle.move(moveSpeed, 0);

        // Reset speedClock if any movement key is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            speedClock.restart();
        }

        // Close window on Esc key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        window.clear();
        circle.draw(window);
        text.setString("Speed: " + std::to_string(speed) + ", Accel: " + std::to_string(acceleration));
        window.draw(text);
        window.display();
    }

    return 0;
}
