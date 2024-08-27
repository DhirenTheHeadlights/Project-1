#include <SFML/Graphics.hpp>
#include <fstream>

#include "json.hpp"


namespace PirateGame {
    static struct SaveData {
        int playerExperience;
        int playerExperienceToLevelUp;
        int playerLevel;
        int playerGold;
        std::vector<std::pair<std::string, std::pair<float, int>>> playerInventory;
    };
    static struct LandmassData {
        float rockScaling;
        float shipwreckScaling;
        float islandScaling;
        int islandMarketSizeLimit;
        int islandMarketItemLimit;
        int islandMarketPriceLimit;
    };
    static struct ShipData {
        sf::Time shipHealthRegenTime;
        sf::Time cannonResetRotationTime;
        sf::Time cannonCooldown;
        sf::Vector2f cannonballScale;
        float cannonballSpeed;
        float cannonballFlightTime;
        float maxFiringAngle;
        float cannonRotationSpeed;
        float turningSpeed;
        float turningMultiplier;
        float frictionCoefficient;
        float dampingFactor;
        float separationDistance;
        float pushOutDistance;
        std::unordered_map<std::string, std::vector<float>> shipEnumProperties;
    };
    static struct ShipGroupData {
        float shipGroupAlignmentWeight;
        float shipGroupCohesionWeight;
        float shipGroupSeparationWeight;
        float shipGroupCombatSpeedMultiplier;
    };
    static struct GameWorldData {
        float sandyShoresScaling;
        float murkyMarshesScaling;
        float devilsDenScaling;
        float shipwreckShoalsScaling;
        float stormySeasScaling;
    };
    static struct GameConfig {
        LandmassData landmassData;
        ShipData shipData;
        ShipGroupData shipGroupData;
        GameWorldData gameWorldData;
    };
    static struct jsonData {
        SaveData saveData;
        GameConfig gameConfig;    
    };

	class JSONLoader {
        using json = nlohmann::json;
	public:
        JSONLoader() {};
        void loadSaveData(const std::string filename) {
			std::ifstream savefile(filename);
            json save = json::parse(savefile);

            jsonValues.saveData.playerExperience = static_cast<int>(save["playerExperience"]);
            jsonValues.saveData.playerExperienceToLevelUp = static_cast<int>(save["playerExperienceToLevelUp"]);
            jsonValues.saveData.playerLevel = static_cast<int>(save["playerLevel"]);
            jsonValues.saveData.playerGold = static_cast<int>(save["playerGold"]);
			for (auto i : save["playerInventory"]) {
				jsonValues.saveData.playerInventory.push_back(std::make_pair(i[0], std::make_pair(i[1], i[2])));
			}
            savefile.close();
        }
        void loadGameConfig(const std::string filename) {
			std::ifstream file(filename);

			json config = json::parse(file);
            jsonValues.gameConfig.landmassData.rockScaling = static_cast<float>(config["rockScaling"]);
            jsonValues.gameConfig.landmassData.shipwreckScaling = static_cast<float>(config["shipwreckScaling"]);
            jsonValues.gameConfig.landmassData.islandScaling = static_cast<float>(config["islandScaling"]);
            jsonValues.gameConfig.landmassData.islandMarketSizeLimit = static_cast<int>(config["islandMarketSizeLimit"]);
            jsonValues.gameConfig.landmassData.islandMarketItemLimit = static_cast<int>(config["islandMarketItemLimit"]);
            jsonValues.gameConfig.landmassData.islandMarketPriceLimit = static_cast<int>(config["islandMarketPriceLimit"]);
            jsonValues.gameConfig.shipData.shipHealthRegenTime = sf::seconds(static_cast<float>(config["shipHealthRegenTime"]));
            jsonValues.gameConfig.shipData.cannonResetRotationTime = sf::seconds(static_cast<float>(config["cannonResetRotationTime"]));
            jsonValues.gameConfig.shipData.cannonCooldown = sf::seconds(static_cast<float>(config["cannonCooldown"]));
            jsonValues.gameConfig.shipData.cannonballScale = sf::Vector2f(static_cast<float>(config["cannonballScale"][0]), static_cast<float>(config["cannonballScale"][1]));
            jsonValues.gameConfig.shipData.cannonballSpeed = static_cast<float>(config["cannonballSpeed"]);
            jsonValues.gameConfig.shipData.cannonballFlightTime = static_cast<float>(config["cannonballFlightTime"]);
            jsonValues.gameConfig.shipData.maxFiringAngle = static_cast<float>(config["maxFiringAngle"]);
            jsonValues.gameConfig.shipData.cannonRotationSpeed = static_cast<float>(config["cannonRotationSpeed"]);
            jsonValues.gameConfig.shipData.turningSpeed = static_cast<float>(config["turningSpeed"]);
            jsonValues.gameConfig.shipData.turningMultiplier = static_cast<float>(config["turningMultiplier"]);
            jsonValues.gameConfig.shipData.frictionCoefficient = static_cast<float>(config["frictionCoefficient"]);
            jsonValues.gameConfig.shipData.dampingFactor = static_cast<float>(config["dampingFactor"]);
            jsonValues.gameConfig.shipData.separationDistance = static_cast<float>(config["separationDistance"]);
            jsonValues.gameConfig.shipData.pushOutDistance = static_cast<float>(config["pushOutDistance"]);
            jsonValues.gameConfig.shipData.shipEnumProperties.insert({"Sloop", (config["shipEnumProperties"]["Sloop"])});
            jsonValues.gameConfig.shipData.shipEnumProperties.insert({ "Brigantine", (config["shipEnumProperties"]["Brigantine"]) });
            jsonValues.gameConfig.shipData.shipEnumProperties.insert({ "Frigate", (config["shipEnumProperties"]["Frigate"]) });
            jsonValues.gameConfig.shipData.shipEnumProperties.insert({ "ManOWar", (config["shipEnumProperties"]["ManOWar"]) });
            jsonValues.gameConfig.shipData.shipEnumProperties.insert({ "Galleon", (config["shipEnumProperties"]["Galleon"]) });
            jsonValues.gameConfig.shipGroupData.shipGroupAlignmentWeight = static_cast<float>(config["shipGroupAlignmentWeight"]);
            jsonValues.gameConfig.shipGroupData.shipGroupCohesionWeight = static_cast<float>(config["shipGroupCohesionWeight"]);
            jsonValues.gameConfig.shipGroupData.shipGroupSeparationWeight = static_cast<float>(config["shipGroupSeparationWeight"]);
            jsonValues.gameConfig.shipGroupData.shipGroupCombatSpeedMultiplier = static_cast<float>(config["shipGroupCombatSpeedMultiplier"]);
            jsonValues.gameConfig.gameWorldData.sandyShoresScaling = static_cast<float>(config["sandyShoresScaling"]);
            jsonValues.gameConfig.gameWorldData.murkyMarshesScaling = static_cast<float>(config["murkyMarshesScaling"]);
            jsonValues.gameConfig.gameWorldData.devilsDenScaling = static_cast<float>(config["devilsDenScaling"]);
            jsonValues.gameConfig.gameWorldData.shipwreckShoalsScaling = static_cast<float>(config["shipwreckShoalsScaling"]);
			jsonValues.gameConfig.gameWorldData.stormySeasScaling = static_cast<float>(config["stormySeasScaling"]);   
            file.close();
        }
	private:
        json saveData;
        jsonData jsonValues;
	};
}