#include "ShipCannonHandler_PG.h"

using namespace PirateGame;

void ShipCannonHandler::initializeCannons(const sf::Texture& cannonTexture, const sf::Image& shipImage, int numCannons, ID* id, sf::Vector2f scale) {
    for (int i = 0; i < numCannons; i++) {
        if (i < numCannons / 2) {
			cannons.push_back(ShipCannon(cannonTexture, id, FiringSide::Port, scale));
		}
        else {
			cannons.push_back(ShipCannon(cannonTexture, id, FiringSide::Starboard, scale));
		}
	}

    // Set the position of the cannons based on the ship sprite. In the ship sprite,
    // There is a 1 pixel red dot where the cannon should be placed.
    const sf::Uint8* pixels = shipImage.getPixelsPtr();

    // Store the positions of the red dots in the ship sprite
    // This should be relative to the ship sprite, so that it moves with the ship
    std::vector<sf::Vector2f> cannonPositions;

    // Find the center of the texture to make positions relative to it
    sf::Vector2f textureCenter = sf::Vector2f(static_cast<float>(shipImage.getSize().x) / 2.0f,
        static_cast<float>(shipImage.getSize().y) / 2.0f);

    // Iterate through the pixels to find the red dots and store their positions relative to the texture center
    for (unsigned int x = 0; x < shipImage.getSize().x; x++) {
        for (unsigned int y = 0; y < shipImage.getSize().y; y++) {
            sf::Color pixelColor = shipImage.getPixel(x, y);
            if (pixelColor.r == 255 && pixelColor.g == 0 && pixelColor.b == 0) {
                // Calculate position relative to the center of the texture and apply scaling
                sf::Vector2f relativeToCenter(x - textureCenter.x, y - textureCenter.y);
                cannonPositions.push_back(sf::Vector2f(relativeToCenter.x * scale.x, relativeToCenter.y * scale.y));
            }
        }
    }

    if (cannonPositions.size() != numCannons) {
        std::cout << "Error: Number of cannons does not match the number of red dots in the ship sprite: " << numCannons << " cannons, " << cannonPositions.size() << " red dots." << std::endl;
        std::cout << "Texture Size : " << shipSprite.getTexture()->getSize().x << " " << shipSprite.getTexture()->getSize().y << std::endl;
        std::cout << "Image Size : " << shipImage.getSize().x << " " << shipImage.getSize().y << std::endl;
    }

    // Set each cannon's offset based on the calculated and scaled positions
    for (int i = 0; i < numCannons; i++) {
        cannons[i].setOffset(cannonPositions[i]);
    }
}

void ShipCannonHandler::shootCannonballs(const sf::Texture& cannonballTexture, GlobalIDManager* GIDM) {
    // Fire the cannonballs with the adjusted direction
    for (auto& cannon : cannons) {
        cannon.fireCannon(side, shipSprite, cannonballTexture, GIDM);
	}
}

void ShipCannonHandler::updateCannons(sf::RenderWindow* window, sf::Time elapsed) {
    for (size_t i = 0; i < cannons.size(); i++) {
        cannons[i].updateCannon(shipSprite, side, window, elapsed);
    }
}

void ShipCannonHandler::drawCannons(sf::RenderWindow* window) {
    for (auto& cannon : cannons) {
        cannon.drawCannonNBalls(window);
	}
}
