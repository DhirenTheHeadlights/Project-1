#pragma once

/// This class is going to handle the land masses in the game.
/// It will be used to generate the land masses and store them in a vector.

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>

#include "GlobalValues_PG.h"
#include "GlobalQuadtreeHandler.h"
#include "GlobalSoundManager_PG.h"
#include "GlobalMap_PG.h"

#include "Island_PG.h"
#include "Rock_PG.h"
#include "Shipwreck_PG.h"
#include "LandMassHashmap_PG.h"
#include "Map.h"

namespace PirateGame {
    class LandMassHandler {
    public:
        LandMassHandler() {};
        ~LandMassHandler();

        void addLandMasses(int numLandMasses, float minDistBetweenLandmasses);
        void createIsland(sf::Vector2f position);
        void createRock(sf::Vector2f position);
        void createShipwreck(sf::Vector2f position);
        void drawLandMasses();
        void interactWithLandmasses();

        // Getters
        std::vector<std::shared_ptr<LandMass>>& getLandMasses() { return landmasses; }

        // Setters
        void setPlayerShip(PlayerShip* playerShip) { this->playerShip = playerShip; }
    private:
        // Helper functions
        void addLandMassesToChunk(Map& map, int numLandMasses, float minDistBetweenLandmasses);

        std::vector<std::shared_ptr<LandMass>> landmasses;
        std::vector<std::shared_ptr<Island>> islands;
        std::vector<std::shared_ptr<Rock>> rocks;
        std::vector<std::shared_ptr<Shipwreck>> shipwrecks;

        Island* nearestIsland = nullptr;
        PlayerShip* playerShip = nullptr;

        // Values
        float interactionDistance = 1000.f; // Distance for interaction with landmasses
		float lootDistance = 300.f; // Distance for looting landmasses
		bool displayLootText = false;
        int displayTextSize = 20;
        int displaySpacing = 100;
        sf::Text lootDisplayText;
        sf::Clock textDisplayClock;
		sf::Time textDisplayTime = sf::seconds(2);
    };
}