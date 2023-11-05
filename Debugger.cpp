#include "Debugger.h"

Debugger::Debugger(sf::RenderWindow& window) {
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
}

void Debugger::drawInformation(sf::RenderWindow& window, const std::string& info, int textSize, float setPosX, float setPosY) {
    sf::Text text;
    text.setFont(font);  // Use the preloaded font
    text.setString(info);
    text.setCharacterSize(textSize); // in pixels, not points!
    text.setFillColor(sf::Color::Red);

    // Center the text based on its local bounds
    float offsetX = text.getLocalBounds().width / 2.0f;
    float offsetY = text.getLocalBounds().height / 2.0f;

    text.setPosition(setPosX - offsetX, setPosY - offsetY);

    window.draw(text);

    //std::cout << "Drawing text\n";
}

void Debugger::drawVector(const sf::Vector2f& start, const sf::Vector2f& direction, sf::RenderWindow& window, float magnitude, sf::Color color) {
    sf::VertexArray lines(sf::Lines, 2);

    lines[0].position = start; // start position
    lines[0].color = color;

    // calculate end position by adding the direction (normalized) multiplied by the magnitude
    lines[1].position = start + direction / std::sqrt(direction.x * direction.x + direction.y * direction.y) * magnitude;
    lines[1].color = color;

    window.draw(lines);

    //std::cout << "Drawing vector\n";
}

void Debugger::drawCell(sf::RenderWindow& window, int x, int y, Map& map, sf::Color color) {
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(map.getCellSize(), map.getCellSize()));
    rectangle.setFillColor(color);
    rectangle.setPosition(x * map.getCellSize(), y * map.getCellSize());
    window.draw(rectangle);

    //std::cout << "Drawing cell\n";
}