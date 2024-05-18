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
        void addLandMassesToChunk(Map& map, int numLandMasses, float minDistBetweenLandmasses);
        void createLandmass(LandMassType type, sf::Vector2f position);

        std::vector<std::shared_ptr<LandMass>> landmasses;
        LandMass* nearestLandMass = nullptr;

        // Values
        float interactionDistance = 1000.f; // Distance for interaction with landmasses
    };
}