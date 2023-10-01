#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>
#include <thread>

sf::RenderWindow window(sf::VideoMode(1920, 1080), "RANGER WINDOW");

sf::Event event;
sf::Clock clockington;
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


double exponential(double x) {
    return exp(x);
}

double speedup(double seconds) {
    double distance = 2.00;
    if (seconds < 2) {
        distance = exponential(distance);
        clockington.restart();
    }
    else {
        distance = 2;
    }
    return distance;
}

void moveUp(double distance) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and y > 0) {
        y -= distance;
    }
}

void moveDown(double distance) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) and y < 1080 - circlesize) {
        y += distance;
    }
}

void moveRight(double distance) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) and x < 1920 - circlesize) {
        x += distance;
    }
}

void moveLeft(double distance) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) and x > 0) {
        x -= distance;
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

    double distance = 1;

    while (window.isOpen()) {

        
        while (window.pollEvent(event)) {

            switch (event.type) {

            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:

                sf::Time elapsed = clockington.getElapsedTime();

                distance = speedup(elapsed.asSeconds());

                moveUp(distance);
                moveDown(distance);
                moveLeft(distance);
                moveRight(distance);
                esc();

                circlesize = atLeast20(circlesize);
                shape.setRadius(circlesize);

                break;

            }
        }

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

