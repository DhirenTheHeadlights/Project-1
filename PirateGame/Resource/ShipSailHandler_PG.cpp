#include "ShipSailHandler_PG.h"

using namespace PirateGame;

void ShipSailHandler::loadSailPositions(ShipClass shipClass, sf::Vector2f scaling) {
	// Grab the sail texture for the ship class
	const std::vector<sf::Texture>& sailTextures = GlobalTextureHandler::getInstance().getShipTextures().getSailTextureManager().getTextureGroup(shipClass);

	for (auto& i : sailTextures) {
		sails.push_back(Sail(i, scaling));
	}

	// Grab the image for the sprite of the ship class
	sf::Image shipImage = GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getImage(shipClass);

	// Iterate through the pixels to find the green dots and store their positions relative to the texture center
	const sf::Uint8* pixels = shipImage.getPixelsPtr();

	// Store the positions of the green dots in the ship sprite
	// 0 starts at the front sail, with the sails going back in order
	std::vector<sf::Vector2f> sailPositions;

	// Find the center of the ship texture to make positions relative to it
	const sf::Texture& shipTexture = GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getTexture(shipClass);
	sf::Vector2f textureCenter = sf::Vector2f(static_cast<float>(shipTexture.getSize().x) / 2.0f,
				static_cast<float>(shipTexture.getSize().y) / 2.0f);

	std::cout << "Texture center: " << textureCenter.x << ", " << textureCenter.y << std::endl;

	// Iterate through the pixels to find the green dots and store their positions relative to the texture center
	for (int x = 0; x < shipImage.getSize().x; x++) {
		for (int y = 0; y < shipImage.getSize().y; y++) {
			sf::Color pixelColor = shipImage.getPixel(x, y);
			if (pixelColor.r == 0 && pixelColor.g == 255 && pixelColor.b == 0) {
				// Calculate position relative to the center of the texture
				sf::Vector2f relativeToCenter(x - textureCenter.x, y - textureCenter.y);
				sailPositions.push_back(sf::Vector2f(relativeToCenter.x * scaling.x, relativeToCenter.y * scaling.y));
			}
		}
	}

	if (sailPositions.size() != sailTextures.size()) {
		std::cout << "Error: Number of sails does not match the number of green dots in the ship sprite." << std::endl;
	}

	// Set each sail's position based on the calculated positions
	for (int i = 0; i < sailTextures.size(); i++) {
		sails[i].setOffset(sailPositions[i]);
	}
}

void ShipSailHandler::moveSailLeftRight(sf::Keyboard::Key sailLeftKey, sf::Keyboard::Key sailRightKey) {
	for (auto& sail : sails) {
		sail.updateSailLeftRight(sailLeftKey, sailRightKey);
	}
}

void ShipSailHandler::moveSailsUpAndDown(sf::Keyboard::Key sailUpKey, sf::Keyboard::Key sailDownKey) {
	for (auto& sail : sails) {
		sail.updateSailUpDown(sailUpKey, sailDownKey);
	}
}

void ShipSailHandler::update(sf::Sprite& shipSprite) {
	for (auto& sail : sails) {
		sail.updateSail(shipSprite);
	}
}

void ShipSailHandler::draw() {
	for (auto& sail : sails) {
		sf::Sprite& sailSprite = sail.getSprite();
		GlobalValues::getInstance().getWindow()->draw(sailSprite);
	}
}