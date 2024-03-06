#include "Minimap_PG.h"

using namespace PirateGame;

void Minimap::draw() {
    sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

    window->draw(minimap);
    window->draw(minimapSprite);

    // Iterate only through visible landmasses
    for (size_t i = 0; i < visibleLandmassRects.size(); ++i) {
        window->draw(visibleLandmassRects[i]);
    }

    window->draw(shipIconSprite);
}

void Minimap::update(float shipRotation) {
    sf::Vector2f mapSize = GlobalValues::getInstance().getMapSize();
    sf::Vector2f shipPos = ship->getSprite().getPosition();
    sf::Vector2f minimapCenter = sf::Vector2f(minimapSprite.getPosition().x + minimapSprite.getGlobalBounds().width / 2, minimapSprite.getPosition().y + minimapSprite.getGlobalBounds().height / 2);

    minimapSprite.setPosition(position);

    // Define the visibility radius around the ship for the minimap
    float visibilityRadius = 10000.0f;

    shipIconSprite.setPosition(minimapCenter);
    shipIconSprite.setOrigin(shipIconSprite.getLocalBounds().width / 2, shipIconSprite.getLocalBounds().height / 2);
    shipIconSprite.setRotation(shipRotation);

    // Clear the previous visible landmasses
    visibleLandmassRects.clear();

    float minimapRadius = minimapSprite.getGlobalBounds().width / 2;

    for (size_t i = 0; i < landmasses.size(); ++i) {
        sf::Vector2f landmassPos = landmasses[i]->getSprite().getPosition();

        // Calculate distance from the ship to the landmass
        float distance = sqrt(pow(shipPos.x - landmassPos.x, 2) + pow(shipPos.y - landmassPos.y, 2));

        if (distance <= visibilityRadius) {
            sf::RectangleShape landmassRect;

            // Adjust position relative to the ship's position on the minimap
            sf::Vector2f scaledPos = (landmassPos - shipPos) * (minimapSprite.getGlobalBounds().height / 2 / visibilityRadius);
            sf::Vector2f relativePos = minimapCenter + scaledPos;

            // Ensure landmassRect is within the minimap's bounds
            if (sqrt(pow(relativePos.x - minimapCenter.x, 2) + pow(relativePos.y - minimapCenter.y, 2)) <= minimapRadius) {
                landmassRect.setPosition(relativePos);

                // Set landmass type properties
                if (landmasses[i]->getType() == LandMassType::Island) {
                    landmassRect.setFillColor(sf::Color::Red);
                    landmassRect.setSize(islandIconSize);
                }
                else if (landmasses[i]->getType() == LandMassType::Rock) {
                    landmassRect.setFillColor(sf::Color::Black);
                    landmassRect.setSize(rockIconSize);
                }
                else if (landmasses[i]->getType() == LandMassType::Shipwreck) {
                    landmassRect.setFillColor(sf::Color::Green);
                }

                visibleLandmassRects.push_back(landmassRect);
            }
        }
    }
}
