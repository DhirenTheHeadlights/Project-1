#include "EnemyShipInputHandler_PG.h"

using namespace PirateGame;

void EnemyShipInputHandler::handleCannonFire() {
	// Only fire if the player is within firingRange
	float distance = static_cast<float>(sqrt(pow(playerPos.x - sprite.getPosition().x, 2) + pow(playerPos.y - sprite.getPosition().y, 2)));
	if (distance > firingDistance) {
		return;
	}
	SCH->shootCannonballs(numCannons, playerPos);
}

void EnemyShipInputHandler::handleCannonAim() {
    // Set the manual aim flag to true
    SCH->setFreeAim(true);

    // Set the firing side of the ship based on the player's position
    // Calculate vector from enemy ship to player ship
    sf::Vector2f directionVector = playerPos - sprite.getPosition();

    // Calculate angle to horizontal in radians
    float angleToHorizontal = atan2(directionVector.y, directionVector.x);

    // Convert radians to degrees
    float angleInDegrees = angleToHorizontal * (180.f / 3.141526f);
    
    // Normalize the angle to the range [0, 360]
    while (angleInDegrees < 0) angleInDegrees += 360.f;
    while (angleInDegrees >= 360.f) angleInDegrees -= 360.f;

    // Determine side based on the adjusted angle
    // This assumes 0 degrees is directly to the right, and degrees increase clockwise.
    if (angleInDegrees > 90.f && angleInDegrees < 270.f) getCannonHandler()->setFiringSide(FiringSide::Port);
    else getCannonHandler()->setFiringSide(FiringSide::Starboard);
}


void EnemyShipInputHandler::handleAnchorDrop() {
	//...
}