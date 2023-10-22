#include "Debug.h"

Debug::Debug(sf::RenderWindow& window) : window(window) {
	font.loadFromFile("times_new_roman.ttf");
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
}

void Debug::drawInformation(sf::RenderWindow& window, const std::string& info, int textSize, float setPosX, float setPosY) {
    sf::Text text;
    text.setFont(font);  // Use the preloaded font
    text.setString(info);
    text.setCharacterSize(textSize);
    text.setFillColor(sf::Color::Red);

    // Center the text based on its local bounds
    float offsetX = text.getLocalBounds().width / 2.0f;
    float offsetY = text.getLocalBounds().height / 2.0f;

    text.setPosition(setPosX - offsetX, setPosY - offsetY);

    window.draw(text);
}

void Debug::drawVector(const sf::Vector2f& start, const sf::Vector2f& direction, sf::RenderWindow& window, float magnitude, sf::Color color) {
    sf::VertexArray lines(sf::Lines, 2);

    lines[0].position = start; // start position
    lines[0].color = color;

    // calculate end position by adding the direction (normalized) multiplied by the magnitude
    lines[1].position = start + direction / std::sqrt(direction.x * direction.x + direction.y * direction.y) * magnitude;
    lines[1].color = color;

    window.draw(lines);
}

void Debug::drawCell(int x, int y, Map& map, sf::Color color) {
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(map.getCellSize(), map.getCellSize()));
    rectangle.setFillColor(color);
    rectangle.setPosition(x * map.getCellSize(), y * map.getCellSize());
    window.draw(rectangle);
}