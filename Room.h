#ifndef ROOM_H
#define ROOM_H

#include <SFML/Graphics.hpp>

class Room {
public:
    Room(); // Default constructor
    Room(float width, float height); // Constructor with parameters
    void setPosition(float x, float y); // Set the room's position
    void draw(sf::RenderWindow& window); // Draw the room
    sf::FloatRect getBounds() const;
private:
    sf::RectangleShape roomShape; // The shape representing the room
};

#endif // ROOM_H
