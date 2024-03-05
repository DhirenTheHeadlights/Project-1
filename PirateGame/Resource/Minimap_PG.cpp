#include "Minimap_PG.h"

using namespace PirateGame;

void Minimap::draw() {
    sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

    window->draw(minimap);

    // Iterate only through visible landmasses
    for (size_t i = 0; i < visibleLandmassRects.size(); ++i) {
        window->draw(visibleLandmassRects[i]);
    }

    window->draw(shipIcon);
}

void Minimap::update() {
    sf::Vector2f mapSize = GlobalValues::getInstance().getMapSize();
    sf::Vector2f shipPos = ship->getSprite().getPosition();
    sf::Vector2f minimapCenter = sf::Vector2f(minimap.getPosition().x + minimap.getRadius(), minimap.getPosition().y + minimap.getRadius());

    minimap.setRadius(size);
    minimap.setPosition(position);
    minimap.setOutlineThickness(2);
    minimap.setOutlineColor(sf::Color::Black);

    // Define the visibility radius around the ship for the minimap
    float visibilityRadius = 10000.0f; 

    // Adjust shipIcon position to be at the center of the minimap
    shipIcon.setPosition(minimapCenter);
    shipIcon.setFillColor(sf::Color::Blue);
    shipIcon.setSize(shipIconSize);

    // Clear the previous visible landmasses
    visibleLandmassRects.clear();

    for (size_t i = 0; i < landmasses.size(); ++i) {
        sf::Vector2f landmassPos = landmasses[i]->getSprite().getPosition();

        // Calculate distance from the ship to the landmass
        float distance = sqrt(pow(shipPos.x - landmassPos.x, 2) + pow(shipPos.y - landmassPos.y, 2));

        if (distance <= visibilityRadius) {
            sf::RectangleShape landmassRect;

            // Adjust position relative to the ship's position on the minimap
            sf::Vector2f scaledPos = (landmassPos - shipPos) * (minimap.getRadius() / visibilityRadius);
            sf::Vector2f relativePos = minimapCenter + scaledPos;

            // Ensure landmassRect is within the minimap's bounds
            if (sqrt(pow(relativePos.x - minimapCenter.x, 2) + pow(relativePos.y - minimapCenter.y, 2)) <= minimap.getRadius()) {
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
