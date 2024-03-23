#pragma once

/// This class is going to handle the land masses in the game.
/// It will be used to generate the land masses and store them in a vector.

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>

#include "GlobalValues_PG.h"
#include "GlobalHashmapHandler.h"
#include "GlobalSoundManager_PG.h"
#include "GlobalMap_PG.h"

#include "LandMass_PG.h"
#include "LandMassHashmap_PG.h"
#include "Map.h"

namespace PirateGame {
    class LandMassHandler {
    public:
        LandMassHandler() {};
        ~LandMassHandler();

        void addLandMasses(int numLandMasses, float minDistBetweenLandmasses);
        void drawLandMasses();
        void interactWithLandmasses(PlayerShip* ship);

        // Getters
        std::vector<std::shared_ptr<LandMass>>& getLandMasses() { return landmasses; }
    private:
        // Helper functions
        void createLandmass(LandMassType type, sf::Vector2f position);

        sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

        std::vector<std::shared_ptr<LandMass>> landmasses;

        // Values
        float interactionDistance = 600.f; // Distance for interaction with landmasses

        // Map to store the land masses
        Map& map = *GlobalMap::getInstance().getMap();

    };
}