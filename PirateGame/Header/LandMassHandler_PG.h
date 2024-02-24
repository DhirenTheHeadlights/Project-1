#pragma once

/// <summary>
/// This class is going to handle the land masses in the game.
/// It will be used to generate the land masses and store them in a vector.
/// </summary>
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
        LandMassHandler();
        ~LandMassHandler();

        void addLandMasses(int numLandMasses, float minDistBetweenLandmasses);
        void drawLandMasses(Ship& ship);
    private:
        // Handle collisions between ship and land masses
        void handleCollisions(Ship& ship);
        bool pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2, unsigned alphaLimit = 5, sf::RenderWindow& window = *GlobalValues::getInstance().getWindow(), bool debug = true);

        // Window pointer
        sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

        // Vector to store the land masses
        std::vector<LandMass*> landMasses;

        // Hashmap to store the land masses
        Hashmap hashmap;

        // Map to store the land masses
        Map& map = GlobalValues::getInstance().getMap();

        // Temporary string for sound file
        std::string soundFile = "Sounds/bonk.wav";

        // SoundManager to play sounds
        SoundManager soundManager;

        // Textures
        Textures texture;
    };
}