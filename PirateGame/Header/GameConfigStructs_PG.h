#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

namespace PirateGame {
    struct SaveData {
        int playerExperience = 0;
        int playerExperienceToLevelUp = 0;
        int playerLevel = 0;
        int playerGold = 0;
        std::vector<std::pair<std::string, std::pair<float, int>>> playerInventory;
    };

    struct LandmassData {
        float rockScaling = 0.0f;
        float shipwreckScaling = 0.0f;
        int shipwreckLootPoolSize = 0;
        int shipwreckLootPoolItemLimit = 0;
        float islandScaling = 0.0f;
        int islandMarketSizeLimit = 0;
        int islandMarketItemLimit = 0;
        int islandMarketPriceLimit = 0;
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
        float cannonballVelocityFallOff;
        float maxFiringAngle;
        float cannonRotationSpeed;
        float minDifferenceBetweenCannonAndTarget;

        // Sail Handler
        float maxSailRotationOffset = 0.0f;
        float sailRotationSpeed = 0.0f;

        // Movement Handler
        float turningSpeed = 0.0f;
        float turningMultiplier = 0.0f;
        float frictionCoefficient = 0.0f;
        float dampingFactor = 0.0f;
        float separationDistance = 0.0f;
        float pushOutDistance = 0.0f;
    };

    struct ShipGroupData {
        float shipGroupAlignmentWeight = 0.0f;
        float shipGroupCohesionWeight = 0.0f;
        float shipGroupSeparationWeight = 0.0f;
        float shipGroupCombatSpeedMultiplier = 0.0f;
    };

    struct GameWorldData {
        float sandyShoresScaling = 0.0f;
        float murkyMarshesScaling = 0.0f;
        float devilsDenScaling = 0.0f;
        float shipwreckShoalsScaling = 0.0f;
        float stormySeasScaling = 0.0f;
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
