#include "ShipCannon_PG.h"

using namespace PirateGame;

void ShipCannon::fireCannon(FiringSide FS, const sf::Sprite& shipSprite, const sf::Texture& cannonballTexture, GlobalIDManager* GIDM) {
    if (FS != side) return;

    // Set the cannonball to come out of the cannon's muzzle
    sf::Vector2f untransformedPositionOutOfCannon = cannonSprite.getPosition() + sf::Vector2f((side == FiringSide::Port) ? cannonSprite.getGlobalBounds().width : 0.f, cannonSprite.getGlobalBounds().height / 2.f);
    sf::Vector2f transformedPositionOutOfCannon = vm::relativeRotationTransformedPosition(untransformedPositionOutOfCannon, sf::Vector2f(0.f, 0.f), cannonSprite.getRotation());

    sf::Vector2f currentFireDirection = vm::normalize(vm::angleDegreesToVector(cannonSprite.getRotation()));

    Cannonball* cannonball = new Cannonball(GIDM, id, cannonballTexture, transformedPositionOutOfCannon, currentFireDirection, jsl->getGameData().gameConfig.shipData.cannonballSpeed, jsl->getGameData().gameConfig.shipData.cannonballScale);

    // Add the cannonball to the management systems
    cannonballHashmap->addObject(cannonball);
    cannonballs.push_back(cannonball);
}

void ShipCannon::drawCannonNBalls(sf::RenderWindow* window) const {
    for (auto& i : cannonballs) {
		window->draw(i->sprite);
	}
    window->draw(cannonSprite);
}

sf::Vector2f ShipCannon::calculatePerpendicularDirection(float rotation) const {
    float rotationInRadians = vm::degreesToRadians(rotation);

    // Calculate the direction of the cannon and normalize it
    sf::Vector2f cannonDirection = vm::normalize(vm::angleRadiansToVector(rotationInRadians));

    cannonDirection *= (side == FiringSide::Starboard) ? -1.f : 1.f;

    return cannonDirection;
}

sf::Vector2f ShipCannon::calculateDirectionToTarget(const sf::Sprite& shipSprite, sf::Vector2f targetPos) {
    sf::Vector2f shipPos = shipSprite.getPosition();

    // Calculate vector from ship to target
    sf::Vector2f directionToTarget = targetPos - shipPos;
    directionToTarget *= (side == FiringSide::Starboard) ? -1.f : 1.f;

    // Calculate angle from ship to target in degrees
    float angleToMouse = vm::vectorToAngleDegrees(directionToTarget);

    // Adjust angle based on ship orientation and capped angle
    float shipRotation = (shipSprite.getRotation() - 180);
    float angleDifference = vm::normalizeAngle((angleToMouse - shipRotation), -180.f, 180.f);

    // Cap the angleDifference within the maxAngle
    angleDifference = std::max(std::min(angleDifference, jsl->getGameData().gameConfig.shipData.maxFiringAngle), -jsl->getGameData().gameConfig.shipData.maxFiringAngle);

    // Calculate final direction vector based on capped angle
    float cappedAngleRadians = vm::degreesToRadians(shipRotation + angleDifference);
    sf::Vector2f cappedDirection = vm::angleRadiansToVector(cappedAngleRadians);

    // Normalize the capped direction vector
    cappedDirection = vm::normalize(cappedDirection);

    // Invert the direction if firing from the starboard side
    cappedDirection *= (side == FiringSide::Starboard) ? -1.f : 1.f;

    return cappedDirection;
}

// Rotate the cannon towards a target angle smoothly
void ShipCannon::rotateTowards(float targetAngle, float step) {
    float currentAngle = cannonSprite.getRotation();
    float angleDifference = vm::normalizeAngle((targetAngle - currentAngle), -180.f, 180.f);

    if (std::fabs(angleDifference) < step) {
        cannonSprite.setRotation(targetAngle);
    }
    else {
        cannonSprite.rotate(step * (angleDifference > 0 ? 1 : -1));
    }
}

void ShipCannon::updateCannonRotation(const sf::Sprite& shipSprite, FiringSide FS, sf::RenderWindow* window) {
    if (FS != side) {
        state = FiringState::Untargeted;
    }

    // Calculate the cannon's position based on the ship's rotation
    cannonSprite.setPosition(vm::relativeRotationTransformedPosition(shipSprite.getPosition(), offset, shipSprite.getRotation()));

    switch (state) {
        case FiringState::TowardsTarget: {
            sf::Vector2f targetDirection = calculateDirectionToTarget(shipSprite, targetPos);
            float targetRotation = vm::vectorToAngleDegrees(targetDirection);
            rotateTowards(targetRotation, jsl->getGameData().gameConfig.shipData.cannonRotationSpeed);

            resetRotationClock.restart();
            break;
        }
        case FiringState::TowardsMouse: {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
            sf::Vector2f worldMousePos = window->mapPixelToCoords(mousePosition);
            sf::Vector2f targetDirection = calculateDirectionToTarget(shipSprite, worldMousePos);
            float targetRotation = vm::vectorToAngleDegrees(targetDirection);

            rotateTowards(targetRotation, jsl->getGameData().gameConfig.shipData.cannonRotationSpeed);

            resetRotationClock.restart();
            break;
        }
        case FiringState::Untargeted: {
            // If firing state is untargeted, slowly rotate towards the default rotation
            if (resetRotationClock.getElapsedTime() < jsl->getGameData().gameConfig.shipData.cannonResetRotationTime) {
                rotateTowards(shipSprite.getRotation() + defaultRotation, jsl->getGameData().gameConfig.shipData.cannonRotationSpeed);
            }
            // Now, rotate towards the default rotation immediately so that the cannon isnt affected by the ship's rotation
            else cannonSprite.setRotation(shipSprite.getRotation() + defaultRotation);
            break;
        }
    }
}

void ShipCannon::updateCannonballs(sf::Time elapsed) {
    for (auto& i : cannonballs) {
        cannonballHashmap->updateObjectPosition(i);

		i->speed = i->speed * pow(jsl->getGameData().gameConfig.shipData.cannonballVelocityFallOff, elapsed.asSeconds());
		i->sprite.move(i->velocity * i->speed * elapsed.asSeconds());

        if (i->clock.getElapsedTime() > jsl->getGameData().gameConfig.shipData.cannonballLifetime) {
            cannonballHashmap->removeObject(i);
            cannonballs.erase(std::remove(cannonballs.begin(), cannonballs.end(), i), cannonballs.end());
        }
	}
}

void ShipCannon::updateCannon(const sf::Sprite& shipSprite, FiringSide FS, sf::RenderWindow* window, sf::Time elapsed) {
    updateCannonRotation(shipSprite, FS, window);
    updateCannonballs(elapsed);
}