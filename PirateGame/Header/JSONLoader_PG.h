#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <unordered_map>
#include "GameConfigStructs_PG.h"
#include "json.hpp"

namespace PirateGame {
    class JSONLoader {
        using json = nlohmann::json;
    public:
        JSONLoader() {};

        void loadSaveData(const std::string& filename) {
            std::ifstream savefile(filename);
            json save = json::parse(savefile);

            parseSection(save["playerExperience"], gameData.saveData.playerExperience);
            parseSection(save["playerExperienceToLevelUp"], gameData.saveData.playerExperienceToLevelUp);
            parseSection(save["playerLevel"], gameData.saveData.playerLevel);
            parseSection(save["playerGold"], gameData.saveData.playerGold);
            parseSection(save["playerInventory"], gameData.saveData.playerInventory);

            savefile.close();
        }

        void loadGameConfig(const std::string& filename) {
            std::ifstream file(filename);
            json config = json::parse(file);

            loadGameWorldData(config["region"]);
            loadLandmassData(config["landmass"]);
            loadShipData(config["ship"]);
            file.close();
        }

        GameData& getGameData() {
            return gameData;
        }

    private:
        template <typename T>
        void parseSection(const json& json, T& section) {
            if (!json.is_null()) section = json.get<T>();
        }

        template <>
        void parseSection(const json& json, sf::Time& section) {
            if (!json.is_null()) section = sf::seconds(json.get<float>());
        }

        template <>
        void parseSection(const json& json, sf::Vector2f& section) {
            if (!json.is_null() && json.is_array() && json.size() == 2) {
                section = sf::Vector2f(json[0], json[1]);
            }
            else {
                std::cerr << "Error: Invalid sf::Vector2f format in JSON." << std::endl;
            }
        }

        template <>
        void parseSection(const json& json, std::unordered_map<std::string, std::vector<float>>& section) {
            for (auto& i : json.items()) {
                if (i.value().is_array()) {
                    section[i.key()] = i.value().get<std::vector<float>>();
                }
                else {
                    std::cerr << "Error: Unexpected format in JSON file for unordered_map<std::string, std::vector<float>>" << std::endl;
                }
            }
        }

        template <>
        void parseSection(const json& json, std::vector<std::pair<std::string, std::pair<float, int>>>& section) {
            for (auto& i : json) {
                if (i.is_array() && i.size() == 3 && i[0].is_string() && i[1].is_number_float() && i[2].is_number_integer()) {
                    section.push_back(std::make_pair(i[0], std::make_pair(i[1], i[2])));
                }
                else {
                    std::cerr << "Error: Invalid format in JSON for vector<std::pair<std::string, std::pair<float, int>>>" << std::endl;
                }
            }
        }

        void loadLandmassData(const json& json) {
            parseSection(json["rockScaling"], gameData.gameConfig.landmassData.rockScaling);
            parseSection(json["shipwreckScaling"], gameData.gameConfig.landmassData.shipwreckScaling);
            parseSection(json["shipwreckLootPoolSize"], gameData.gameConfig.landmassData.shipwreckLootPoolSize);
            parseSection(json["shipwreckLootPoolItemLimit"], gameData.gameConfig.landmassData.shipwreckLootPoolItemLimit);
            parseSection(json["islandScaling"], gameData.gameConfig.landmassData.islandScaling);
            parseSection(json["islandMarketSizeLimit"], gameData.gameConfig.landmassData.islandMarketSizeLimit);
            parseSection(json["islandMarketItemLimit"], gameData.gameConfig.landmassData.islandMarketItemLimit);
            parseSection(json["islandMarketPriceLimit"], gameData.gameConfig.landmassData.islandMarketPriceLimit);
			parseSection(json["landmassInteractionDistance"], gameData.gameConfig.landmassData.landmassInteractionDistance);
			parseSection(json["shipwreckInteractionDistance"], gameData.gameConfig.landmassData.shipwreckInteractionDistance);
        }

        void loadShipData(const json& json) {
            auto baseJson = json["base"];
            parseSection(baseJson["shipHealthRegenTime"], gameData.gameConfig.shipData.shipHealthRegenTime);
            parseSection(baseJson["shipHealthBarSize"], gameData.gameConfig.shipData.shipHealthBarSize);
            parseSection(baseJson["shipEnumProperties"], gameData.gameConfig.shipData.shipEnumProperties);

            auto cannonJson = json["cannonHandler"];
            parseSection(cannonJson["cannonResetRotationTime"], gameData.gameConfig.shipData.cannonResetRotationTime);
            parseSection(cannonJson["cannonCooldown"], gameData.gameConfig.shipData.cannonCooldown);
            parseSection(cannonJson["cannonballScale"], gameData.gameConfig.shipData.cannonballScale);
            parseSection(cannonJson["cannonballSpeed"], gameData.gameConfig.shipData.cannonballSpeed);
            parseSection(cannonJson["cannonballVelocityFallOff"], gameData.gameConfig.shipData.cannonballVelocityFallOff);
            parseSection(cannonJson["cannonballLifetime"], gameData.gameConfig.shipData.cannonballLifetime);
            parseSection(cannonJson["maxFiringAngle"], gameData.gameConfig.shipData.maxFiringAngle);
            parseSection(cannonJson["cannonRotationSpeed"], gameData.gameConfig.shipData.cannonRotationSpeed);
            parseSection(cannonJson["minDifferenceBetweenCannonAndTarget"], gameData.gameConfig.shipData.minDifferenceBetweenCannonAndTarget);

            auto sailJson = json["sailHandler"];
            parseSection(sailJson["maxSailRotationOffset"], gameData.gameConfig.shipData.maxSailRotationOffset);
            parseSection(sailJson["sailRotationSpeed"], gameData.gameConfig.shipData.sailRotationSpeed);

            auto movementJson = json["movementHandler"];
            parseSection(movementJson["turningSpeed"], gameData.gameConfig.shipData.turningSpeed);
            parseSection(movementJson["turningMultiplier"], gameData.gameConfig.shipData.turningMultiplier);
            parseSection(movementJson["frictionCoefficient"], gameData.gameConfig.shipData.frictionCoefficient);
            parseSection(movementJson["dampingFactor"], gameData.gameConfig.shipData.dampingFactor);
            parseSection(movementJson["separationDistance"], gameData.gameConfig.shipData.separationDistance);
            parseSection(movementJson["pushOutDistance"], gameData.gameConfig.shipData.pushOutDistance);

            auto shipGroupJson = json["shipGroup"];
            parseSection(shipGroupJson["shipGroupAlignmentWeight"], gameData.gameConfig.shipGroupData.shipGroupAlignmentWeight);
            parseSection(shipGroupJson["shipGroupCohesionWeight"], gameData.gameConfig.shipGroupData.shipGroupCohesionWeight);
            parseSection(shipGroupJson["shipGroupSeparationWeight"], gameData.gameConfig.shipGroupData.shipGroupSeparationWeight);
            parseSection(shipGroupJson["shipGroupCombatSpeedMultiplier"], gameData.gameConfig.shipGroupData.shipGroupCombatSpeedMultiplier);
        }

        void loadGameWorldData(const json& json) {
            parseSection(json["sandyShoresScaling"], gameData.gameConfig.gameWorldData.sandyShoresScaling);
            parseSection(json["murkyMarshesScaling"], gameData.gameConfig.gameWorldData.murkyMarshesScaling);
            parseSection(json["devilsDenScaling"], gameData.gameConfig.gameWorldData.devilsDenScaling);
            parseSection(json["shipwreckShoalsScaling"], gameData.gameConfig.gameWorldData.shipwreckShoalsScaling);
            parseSection(json["stormySeasScaling"], gameData.gameConfig.gameWorldData.stormySeasScaling);
        }

        json saveData;
        GameData gameData;
    };
}
