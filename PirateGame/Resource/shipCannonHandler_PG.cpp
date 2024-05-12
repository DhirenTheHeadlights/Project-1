#include "shipCannonHandler_PG.h"

using namespace PirateGame;

void ShipCannonHandler::initializeCannons(ShipClass type, int numCannons, int ID, sf::Vector2f scale) {
    for (int i = 0; i < numCannons; i++) {
        if (i < numCannons / 2) {
			cannons.push_back(ShipCannon(type, ID, FiringSide::Port, scale));
		}
        else {
			cannons.push_back(ShipCannon(type, ID, FiringSide::Starboard, scale));
		}
	}

    // Set the position of the cannons based on the ship sprite. In the ship sprite,
    // There is a 1 pixel red dot where the cannon should be placed.
    const sf::Texture* shipTexture = shipSprite.getTexture();
    sf::Image shipImage = GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getImage(type);
    const sf::Uint8* pixels = shipImage.getPixelsPtr();

    // Store the positions of the red dots in the ship sprite
    // This should be relative to the ship sprite, so that it moves with the ship
    std::vector<sf::Vector2f> cannonPositions;

    // Find the center of the texture to make positions relative to it
    sf::Vector2f textureCenter = sf::Vector2f(static_cast<float>(shipTexture->getSize().x) / 2.0f,
        static_cast<float>(shipTexture->getSize().y) / 2.0f);

    // Iterate through the pixels to find the red dots and store their positions relative to the texture center
    for (int x = 0; x < shipImage.getSize().x; x++) {
        for (int y = 0; y < shipImage.getSize().y; y++) {
            sf::Color pixelColor = shipImage.getPixel(x, y);
            if (pixelColor.r == 255 && pixelColor.g == 0 && pixelColor.b == 0) {
                // Calculate position relative to the center of the texture and apply scaling
                sf::Vector2f relativeToCenter(x - textureCenter.x, y - textureCenter.y);
                cannonPositions.push_back(sf::Vector2f(relativeToCenter.x * scale.x, relativeToCenter.y * scale.y));
            }
        }
    }

    if (cannonPositions.size() != numCannons) {
        std::cout << "Error: Number of cannons does not match the number of red dots in the ship sprite." << std::endl;
    }

    // Set each cannon's offset based on the calculated and scaled positions
    for (int i = 0; i < numCannons; i++) {
        cannons[i].setOffset(cannonPositions[i]);
        std::cout << "Cannon " << i << " offset: " << cannonPositions[i].x << ", " << cannonPositions[i].y << std::endl;
    }
}

void ShipCannonHandler::shootCannonballs(sf::Vector2f targetPos) {
    if (cannonCooldownClock.getElapsedTime().asSeconds() < cooldown) return;

    // Grab the cannon direction
    sf::Vector2f cannonDirection = this->cannonDirection(targetPos);

    // Fire the cannonballs with the adjusted direction
    for (auto& cannon : cannons) {
		cannon.fireCannon(cannonDirection);
	}

    // Play sound once per volley if numCannons > 0
    if (inAudioRange) GlobalSoundManager::getInstance().playSound(SoundId::CannonShot);

    // Reset the cooldown clock
    cannonCooldownClock.restart();
}

sf::Vector2f ShipCannonHandler::cannonDirection(sf::Vector2f targetPos) {
    const float pi = 3.14159265f;
    if (!aimTowardsMouse) {
        float rotationInRadians = (shipSprite.getRotation() - 180.f) * pi / 180.f;

        // Calculate the direction of the cannon and normalize it
        sf::Vector2f cannonDirection(cos(rotationInRadians), sin(rotationInRadians));
        float magnitude = sqrt(cannonDirection.x * cannonDirection.x + cannonDirection.y * cannonDirection.y);
        cannonDirection /= magnitude;

        cannonDirection *= (side == FiringSide::Starboard) ? -1.f : 1.f;

        return cannonDirection;
    }
    else {
        sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
        sf::Vector2f shipPos = shipSprite.getPosition();

        // Calculate vector from ship to target
        sf::Vector2f directionToTarget = targetPos - shipPos;
        directionToTarget *= (side == FiringSide::Starboard) ? -1.f : 1.f;

        // Calculate angle from ship to target in degrees
        float angleToMouse = atan2(directionToTarget.y, directionToTarget.x) * 180 / pi;

        // Adjust angle based on ship orientation and capped angle
        float shipRotation = (shipSprite.getRotation() - 180);
        float angleDifference = angleToMouse - shipRotation;

        // Ensure angleDifference is within -180 to 180 degrees for correct comparison
        if (angleDifference > 180) angleDifference -= 360;
        else if (angleDifference < -180) angleDifference += 360;

        firingDirectionAngle = angleDifference; // This value is grabbed by the enemy ship input handler
 
        // Cap the angleDifference within the maxAngle
        angleDifference = std::max(std::min(angleDifference, maxFiringAngle), -maxFiringAngle);

        // Calculate final direction vector based on capped angle
        float cappedAngleRadians = (shipRotation + angleDifference) * pi / 180;
        sf::Vector2f cappedDirection(cos(cappedAngleRadians), sin(cappedAngleRadians));

        // Normalize the capped direction vector
        float magnitude = sqrt(cappedDirection.x * cappedDirection.x + cappedDirection.y * cappedDirection.y);
        cappedDirection /= magnitude;

        // Invert the direction if firing from the starboard side
        cappedDirection *= (side == FiringSide::Starboard) ? -1.f : 1.f;

        return cappedDirection;
    }
}

void ShipCannonHandler::updateCannons() {
    for (size_t i = 0; i < cannons.size(); i++) {
        cannons[i].updateCannon(shipSprite);
    }
}

void ShipCannonHandler::drawCannons() {
    for (auto& cannon : cannons) {
        cannon.drawCannon();
	}
}