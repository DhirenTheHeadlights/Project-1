#pragma once

/// This class is going to handle the land masses in the game.
/// It will be used to generate the land masses and store them in a vector.

#include <SFML/Graphics.hpp>
#include <vector>

#include "GlobalContext_PG.h"
#include "GlobalQuadtreeHandler_PG.h"

#include "Island_PG.h"
#include "Rock_PG.h"
#include "Shipwreck_PG.h"

namespace PirateGame {
    class LandMassHandler {
    public:
        LandMassHandler(GlobalContext& context, GlobalQuadtreeHandler* GQH) : context(context), GQH(GQH) {};
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
        GlobalContext& context;
        GlobalQuadtreeHandler* GQH = nullptr;

        // Helper functions
        void addLandMassesToChunk(const Map& map, int numLandMasses, float minDistBetweenLandmasses);

        std::vector<std::shared_ptr<LandMass>> landmasses;
        std::vector<std::shared_ptr<Island>> islands;
        std::vector<std::shared_ptr<Rock>> rocks;
        std::vector<std::shared_ptr<Shipwreck>> shipwrecks;

        Island* nearestIsland = nullptr;
        PlayerShip* playerShip = nullptr;

        // Values
        float interactionDistance = 1000.f; // Distance for interaction with landmasses
		float lootDistance = 300.f; // Distance for looting landmasses
    };
}