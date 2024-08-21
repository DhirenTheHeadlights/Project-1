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
        float shipwreckShoalScaling;
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
            json save = json::parse(filename);
            jsonValues.saveData.playerExperience = save["playerExperience"];
            jsonValues.saveData.playerExperienceToLevelUp = save["playerExperienceToLevelUp"];
            jsonValues.saveData.playerLevel = save["playerLevel"];
            jsonValues.saveData.playerGold = save["playerGold"];

        }
        void loadGameConfig(const std::string filename);
	private:
        json saveData;
        jsonData jsonValues;
	};
}