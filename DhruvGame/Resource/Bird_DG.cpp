#include "Bird_DG.h"

using namespace DhruvGame;

void Bird::initializeBird() {
	static sf::Texture texture;
    if (!texture.loadFromFile("DhruvGame/Sprites/bird.png")) {
        std::cerr << "Failed to load bird texture" << std::endl;
    }
    else {
        sprite.setTexture(texture);
        sprite.setPosition(100, 440);
        sprite.setScale(scaleSize, scaleSize);
        boundingBoxUpper.setSize(sf::Vector2f(16 * scaleSize, 10 * scaleSize));
        boundingBoxMid.setSize(sf::Vector2f(22 * scaleSize, 5 * scaleSize));
        boundingBoxLower.setSize(sf::Vector2f(32 * scaleSize, 3 * scaleSize));
        boundingBoxLower.setOutlineColor(sf::Color::Red);
        boundingBoxMid.setOutlineColor(sf::Color::Blue);
        boundingBoxUpper.setOutlineColor(sf::Color::White);
        lowerOffset *= scaleSize;
        midOffset *= scaleSize;
        upperOffset *= scaleSize;

        std::cout << "lowerOffset: " << lowerOffset.x << ", " << lowerOffset.y << std::endl;
        std::cout << "midOffset: " << midOffset.x << ", " << midOffset.y << std::endl;
        std::cout << "upperOffset: " << upperOffset.x << ", " << upperOffset.y << std::endl;

    }
}

void Bird::moveBird() {
    sprite.move(4, gravity);
    boundingBoxLower.setPosition(sprite.getPosition() + lowerOffset);
    boundingBoxMid.setPosition(sprite.getPosition() + midOffset);
    boundingBoxUpper.setPosition(sprite.getPosition() + upperOffset);
}
