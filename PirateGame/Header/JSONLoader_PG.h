#include <SFML/Graphics.hpp>
#include <fstream>

#include "GameConfigStructs_PG.h"
#include "json.hpp"


namespace PirateGame {
	class JSONLoader {
        using json = nlohmann::json;
	public:
        JSONLoader() {};
        void loadSaveData(const std::string filename) {
			std::ifstream savefile(filename);
            json save = json::parse(savefile);

            parseSection(save["playerExperience"], gameData.saveData.playerExperience);
            parseSection(save["playerExperienceToLevelUp"], gameData.saveData.playerExperienceToLevelUp);
            parseSection(save["playerLevel"], gameData.saveData.playerLevel);
            parseSection(save["playerGold"], gameData.saveData.playerGold);
            parseSection(save["playerInventory"], gameData.saveData.playerInventory);

            savefile.close();
        }
        void loadGameConfig(const std::string filename) {
			std::ifstream file(filename);

			json config = json::parse(file);
            loadGameWorldData(config);
            loadLandmassData(config);
            loadShipData(config);
            loadShipGroupData(config);
            file.close();
        }
	private:
        // Template for loading JSON files
        template <typename T>
        void parseSection(const nlohmann::json& json, T& section) {
            if (!json.is_null()) section = json.get<T>();
        }

        template <>
        void parseSection(const nlohmann::json& json, sf::Time& section) {
            if (!json.is_null()) section = sf::seconds(json.get<float>());
		}

        template <>
        void parseSection(const nlohmann::json& json, sf::Vector2f& section) {
            if (!json.is_null()) section = sf::Vector2f(json[0], json[1]);
        }

        template <>
        void parseSection(const nlohmann::json& json, std::unordered_map<std::string, std::vector<float>>& section) {
            for (auto& i : json) {
                if (i.is_array() && i.size() == 2 && i[0].is_string() && i[1].is_array() && i[1].size() > 0) {
                    section.insert({ i[0].get<std::string>(), i[1].get<std::vector<float>>() });
                }
                else {
                    // Handle error or unexpected format
                    std::cerr << "Error: Unexpected format in JSON file when parsing for unordered_map<std::string, std::vector<float>>" << std::endl;
                }
            }
        }

        template <>
        void parseSection(const nlohmann::json& json, std::vector<std::pair<std::string, std::pair<float, int>>>& section) {
            for (auto i : json) {
                if (i.is_array() && i.size() == 3 && i[0].is_string() && i[1].is_number_float() && i[2].is_number_integer()) {
                    section.push_back(std::make_pair(i[0], std::make_pair(i[1], i[2])));
                }
				else {
					// Handle error or unexpected format
					std::cerr << "Error: Unexpected format in JSON file when parsing for vector<std::pair<std::string, std::pair<float, int>>>" << std::endl;
				}
            }
        }

        void loadLandmassData(const nlohmann::json& json) {
			parseSection(json["rockScaling"], gameData.gameConfig.landmassData.rockScaling);
			parseSection(json["shipwreckScaling"], gameData.gameConfig.landmassData.shipwreckScaling);
			parseSection(json["islandScaling"], gameData.gameConfig.landmassData.islandScaling);
			parseSection(json["islandMarketSizeLimit"], gameData.gameConfig.landmassData.islandMarketSizeLimit);
			parseSection(json["islandMarketItemLimit"], gameData.gameConfig.landmassData.islandMarketItemLimit);
			parseSection(json["islandMarketPriceLimit"], gameData.gameConfig.landmassData.islandMarketPriceLimit);
		}

        void loadShipData(const nlohmann::json& json) {
            parseSection(json["shipHealthRegenTime"], gameData.gameConfig.shipData.shipHealthRegenTime);
            parseSection(json["cannonResetRotationTime"], gameData.gameConfig.shipData.cannonResetRotationTime);
            parseSection(json["cannonCooldown"], gameData.gameConfig.shipData.cannonCooldown);
            parseSection(json["cannonballScale"], gameData.gameConfig.shipData.cannonballScale);
            parseSection(json["cannonballSpeed"], gameData.gameConfig.shipData.cannonballSpeed);
            parseSection(json["cannonballFlightTime"], gameData.gameConfig.shipData.cannonballFlightTime);
            parseSection(json["maxFiringAngle"], gameData.gameConfig.shipData.maxFiringAngle);
            parseSection(json["cannonRotationSpeed"], gameData.gameConfig.shipData.cannonRotationSpeed);
            parseSection(json["turningSpeed"], gameData.gameConfig.shipData.turningSpeed);
            parseSection(json["turningMultiplier"], gameData.gameConfig.shipData.turningMultiplier);
            parseSection(json["frictionCoefficient"], gameData.gameConfig.shipData.frictionCoefficient);
            parseSection(json["dampingFactor"], gameData.gameConfig.shipData.dampingFactor);
            parseSection(json["separationDistance"], gameData.gameConfig.shipData.separationDistance);
            parseSection(json["pushOutDistance"], gameData.gameConfig.shipData.pushOutDistance);
            parseSection(json["shipEnumProperties"], gameData.gameConfig.shipData.shipEnumProperties);
        }

        void loadShipGroupData(const nlohmann::json& json) {
			parseSection(json["shipGroupAlignmentWeight"], gameData.gameConfig.shipGroupData.shipGroupAlignmentWeight);
			parseSection(json["shipGroupCohesionWeight"], gameData.gameConfig.shipGroupData.shipGroupCohesionWeight);
			parseSection(json["shipGroupSeparationWeight"], gameData.gameConfig.shipGroupData.shipGroupSeparationWeight);
			parseSection(json["shipGroupCombatSpeedMultiplier"], gameData.gameConfig.shipGroupData.shipGroupCombatSpeedMultiplier);
		}

        void loadGameWorldData(const nlohmann::json& json) {
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