#include "EnemyShipMovementHandler_PG.h"


using namespace PirateGame;

void EnemyShipMovementHandler::update(float baseSpeed, sf::Vector2f sailDirection) {
	setBaseSpeed(baseSpeed * enemySpeedMultiplier);

    getAStar().update(getSprite().getPosition());
    //getAStar().drawDebug(GlobalValues::getInstance().getWindow());

	move(baseSpeed, sailDirection);
}

void EnemyShipMovementHandler::setSpriteRotation() {
    if (getStopShipRotationFlag()) return;

    //GlobalValues::getInstance().getWindow()->draw(vm::createVectorLine(getSprite().getPosition(), vm::normalize(destination - getSprite().getPosition()) * 200.f, sf::Color::Green));
    //GlobalValues::getInstance().getWindow()->draw(vm::createVectorLine(getSprite().getPosition(), vm::normalize(direction) * 200.f, sf::Color::Red));

    // Check if the ship is active towards the target
    if (activeTowardsTarget) {
        sf::Vector2f directionToTarget = targetPos - getSprite().getPosition();
        float distance = vm::magnitude(directionToTarget);
        if (distance < broadsideDistance) { // Go perpendicular
            direction = sf::Vector2f(directionToTarget.y, -directionToTarget.x);
        }
    }
    else {
        // Get the next point from the A* algorithm
        direction = getAStar().getNextPoint(getSprite().getPosition()) - getSprite().getPosition();
    }

    setTurningMultiplier(activeTowardsTarget ? 1.5f : 1.f);

    float targetAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(vm::normalize(direction)) + 90.f); // +90 to account for sprite rotation
    rotateTowards(targetAngle);
}
