#include "ShipSailHandler_PG.h"

using namespace PirateGame;

void ShipSailHandler::loadSailPositions(const std::vector<sf::Texture>& sailTextures, const sf::Image& shipImage, sf::Vector2f scaling) {
	for (auto& i : sailTextures) {
		sails.push_back(Sail(i, scaling));
	}

	// Iterate through the pixels to find the green dots and store their positions relative to the texture center
	const sf::Uint8* pixels = shipImage.getPixelsPtr();

	// Store the positions of the green dots in the ship sprite
	// 0 starts at the front sail, with the sails going back in order
	std::vector<sf::Vector2f> sailPositions;

	// Find the center of the ship texture to make positions relative to it
	sf::Vector2f textureCenter = sf::Vector2f(static_cast<float>(shipImage.getSize().x) / 2.0f,
				static_cast<float>(shipImage.getSize().y) / 2.0f);

	// Iterate through the pixels to find the green dots and store their positions relative to the texture center
	for (unsigned int x = 0; x < shipImage.getSize().x; x++) {
		for (unsigned int y = 0; y < shipImage.getSize().y; y++) {
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
	// 0 starts at the back sail, with the sails going forward in order
	for (int i = 0; i < sailTextures.size(); i++) {
		sails[i].setOffset(sailPositions[i]);
	}
}

void ShipSailHandler::moveSailLeftRightAutomatically(sf::Vector2f windDirection, sf::Vector2f shipDirection) {
	for (auto& sail : sails) {
		sail.updateSailLeftRightAutomatically(windDirection, shipDirection);
	}
}

void ShipSailHandler::update(const sf::Sprite& shipSprite, sf::Vector2f& shipDirection) {
	for (auto& sail : sails) {
		sail.updateSail(shipSprite, shipDirection);
	}
}

void ShipSailHandler::draw(sf::RenderWindow* window) {
	for (auto& sail : sails) {
		window->draw(sail.getSprite());
	}
}