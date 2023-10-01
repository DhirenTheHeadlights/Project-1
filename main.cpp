#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>

sf::RenderWindow window(sf::VideoMode(1920, 1080), "RANGER WINDOW");

sf::Event event;
sf::Clock mainTime;
sf::Clock speedClock;

sf::Text text;
sf::Font font;

sf::CircleShape shape(300.f);
int originx{ 0 };
int originy{ 0 };

int x{ originx };
int y{ originy };

float circlesize = 300;

int atLeast20(int input) {
    if (input > 20) {
        return input - 20;
    }
    else {
        return input;
    }
}

double acceleration = 1.0;  // Starts at 1.0 and increases while keys are pressed

double speedup() {
    if (speedClock.getElapsedTime().asSeconds() >= 2) {
        acceleration = 1.0;  // Reset acceleration if no key pressed for 2 seconds
    }
    return acceleration;
}

void updateAcceleration() {
    if (acceleration < 5.0) {  // Cap the maximum acceleration value at 5.0
        acceleration += 0.001;  // Increase acceleration while key is pressed
    }
}

void moveUp(double distance) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and y > 0) {
        y -= distance + acceleration;
        speedClock.restart();
        updateAcceleration();
    }
}

void moveDown(double distance) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) and y < 1080 - circlesize) {
        y += distance + acceleration;
        speedClock.restart();
        updateAcceleration();
    }
}

void moveRight(double distance) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) and x < 1920 - circlesize) {
        x += distance + acceleration;
        speedClock.restart();
        updateAcceleration();
    }
}

void moveLeft(double distance) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) and x > 0) {
        x -= distance + acceleration;
        speedClock.restart();
        updateAcceleration();
    }
}

void esc() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }
}

int main() {
    shape.setPosition(originx, originy);
    shape.setFillColor(sf::Color::Blue);

    if (!font.loadFromFile("times_new_roman.ttf")) {
        window.close();
    }

    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Red);

    double baseSpeed = 2.0;  // Base speed in pixels per frame

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                circlesize = atLeast20(circlesize);
                shape.setRadius(circlesize);
                break;
            }
        }

        acceleration = speedup();  // Update acceleration

        moveUp(baseSpeed);
        moveDown(baseSpeed);
        moveLeft(baseSpeed);
        moveRight(baseSpeed);
        esc();

        window.clear();
        window.draw(shape);
        shape.setPosition(float(x), float(y));
        std::string position = std::to_string(x) + "," + std::to_string(y);
        text.setString(position);
        text.setPosition(x, y);
        window.draw(text);
        window.display();
    }

    return 0;
}


