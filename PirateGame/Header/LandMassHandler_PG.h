#pragma once

/// <summary>
/// This class is going to handle the land masses in the game.
/// It will be used to generate the land masses and store them in a vector.
/// </summary>
/// 
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>

#include "GlobalValues_PG.h"
#include "Textures_PG.h"
#include "LandMass_PG.h"
#include "Hashmap_PG.h"
#include "Map.h"
#include "SoundManager_PG.h"

namespace PirateGame {
    class LandMassHandler {
    public:
        LandMassHandler() { soundManager.loadSoundBuffer("bonk", soundFile); };
        ~LandMassHandler();

        void addLandMasses(int numLandMasses, float minDistBetweenLandmasses);
        void drawLandMasses();
        void interactWithLandmasses(Ship& ship);
    private:
        // Helper functions
        bool pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2, unsigned alphaLimit = 5);
        void handleCollisions(Ship& ship, std::set<LandMass*> nearbyLandMasses);
        void createLandmass(LandMassType type, float minDistBetweenLandmasses);
        void openMarket(Ship& ship, LandMass* landMass);

        sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

        std::vector<LandMass*> landMasses;

        // Hashmap and Textures
        Hashmap hashmap;
        Textures texture;

        // Values
        float collisionDamagePerSecond = 4.f;
        float interactionDistance = 600.f; // Distance for interaction with landmasses

        // Map to store the land masses
        Map& map = GlobalValues::getInstance().getMap();

        // Temporary string for sound file
        std::string soundFile = "Sounds/bonk.wav";

        // SoundManager to play sounds
        SoundManager soundManager;

    };
}