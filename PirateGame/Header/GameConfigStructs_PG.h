#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

namespace PirateGame {
    struct SaveData {
        int playerExperience;
        int playerExperienceToLevelUp;
        int playerLevel;
        int playerGold;
        std::vector<std::pair<std::string, std::pair<float, int>>> playerInventory;
    };

    struct LandmassData {
        float rockScaling;
        float shipwreckScaling;
        int shipwreckLootPoolSize;
        int shipwreckLootPoolItemLimit;
        float islandScaling;
        int islandMarketSizeLimit;
        int islandMarketItemLimit;
        int islandMarketPriceLimit;
    };

    struct ShipData {
        // Ship
        sf::Time shipHealthRegenTime;
        sf::Vector2f shipHealthBarSize;
        std::unordered_map<std::string, std::vector<float>> shipEnumProperties;

        // Cannon Handler
        sf::Time cannonResetRotationTime;
        sf::Time cannonCooldown;
        sf::Time cannonballLifetime;
        sf::Vector2f cannonballScale;
        float cannonballSpeed;
        float cannonballFlightTime;
        float cannonballVelocityFallOff;
        float maxFiringAngle;
        float cannonRotationSpeed;
        float minDifferenceBetweenCannonAndTarget;

        // Sail Handler
        float maxSailRotationOffset;
        float sailRotationSpeed;

        // Movement Handler
        float turningSpeed;
        float turningMultiplier;
        float frictionCoefficient;
        float dampingFactor;
        float separationDistance;
        float pushOutDistance;
    };

    struct ShipGroupData {
        float shipGroupAlignmentWeight;
        float shipGroupCohesionWeight;
        float shipGroupSeparationWeight;
        float shipGroupCombatSpeedMultiplier;
    };

    struct GameWorldData {
        float sandyShoresScaling;
        float murkyMarshesScaling;
        float devilsDenScaling;
        float shipwreckShoalsScaling;
        float stormySeasScaling;
    };

    struct GameConfig {
        LandmassData landmassData;
        ShipData shipData;
        ShipGroupData shipGroupData;
        GameWorldData gameWorldData;
    };

    struct GameData {
        SaveData saveData;
        GameConfig gameConfig;
    };
}
