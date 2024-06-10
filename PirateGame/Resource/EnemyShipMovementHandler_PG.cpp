#include "EnemyShipMovementHandler_PG.h"


using namespace PirateGame;

void EnemyShipMovementHandler::update(float baseSpeed, sf::Vector2f sailDirection) {
	setBaseSpeed(baseSpeed * enemySpeedMultiplier);

    getAStar().update(getSprite().getPosition());

	move(baseSpeed, sailDirection);
}

void EnemyShipMovementHandler::setSpriteRotation() {
    if (getStopShipRotationFlag()) return;

    // Use A* to find the next node towards the checkpoint
    direction = getAStar().getNextPoint(getSprite().getPosition()) - getSprite().getPosition();\

    GlobalValues::getInstance().getWindow()->draw(vm::createVectorLine(getSprite().getPosition(), vm::normalize(destination - getSprite().getPosition()) * 100.f, sf::Color::Green));
    GlobalValues::getInstance().getWindow()->draw(vm::createVectorLine(getSprite().getPosition(), vm::normalize(direction) * 100.f, sf::Color::Red));
    GlobalValues::getInstance().displayText("num nearby sprites: " + std::to_string(getNearbySprites().size()), getSprite().getPosition() + sf::Vector2f(0, -50.f), sf::Color::White);

    // Check if the ship is active towards the target
    if (activeTowardsTarget) {
        float distance = vm::magnitude(targetPos - getSprite().getPosition());
        if (distance < broadsideDistance) { // Go perpendicular
            direction = sf::Vector2f(targetPos.y, -targetPos.x) - getSprite().getPosition();
        }
    }

    setTurningMultiplier(activeTowardsTarget ? 1.5f : 1.f);

    float targetAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(vm::normalize(direction)) + 90.f); // +90 to account for sprite rotation
    rotateTowards(targetAngle);
}
