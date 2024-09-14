#pragma once

/// This class just holds the ship enums and properties.

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "GameConfigStructs_PG.h"

namespace PirateGame {
	constexpr int numShipClasses = 5;
	enum class ShipClass {
		Random,
		Sloop,
		Brigantine,
		Frigate,
		ManOWar,
		Galleon
	};
	enum class FiringSide { Port, Starboard };

	// Struct to hold ship properties
	struct ShipProperties {
		ShipProperties() {};

		explicit ShipProperties(const std::vector<float>& properties) {
			maxHealth = properties[0];
			baseSpeed = properties[1];
			regenRate = properties[2];
			scaleX = properties[3];
			scaleY = properties[4];
			numCannons = properties[5];
			price = properties[6];
		}
		float baseSpeed = 1.f;
		float maxHealth = 100.f;
		float regenRate = 0.5f;
		float scaleX = 1.f, scaleY = 1.f;
		int numCannons = 1;
		float price = 0.f;
	};

	inline std::unordered_map<ShipClass, ShipProperties> shipConfig;

	static void setShipConfig(GameConfig& gameConfig) {
		shipConfig.insert(std::make_pair(ShipClass::Sloop, gameConfig.shipData.shipEnumProperties["Sloop"]));
		shipConfig.insert(std::make_pair(ShipClass::Brigantine, gameConfig.shipData.shipEnumProperties["Brigantine"]));
		shipConfig.insert(std::make_pair(ShipClass::Frigate, gameConfig.shipData.shipEnumProperties["Frigate"]));
		shipConfig.insert(std::make_pair(ShipClass::ManOWar, gameConfig.shipData.shipEnumProperties["ManOWar"]));
		shipConfig.insert(std::make_pair(ShipClass::Galleon, gameConfig.shipData.shipEnumProperties["Galleon"]));
	}

	inline ShipProperties getShipProperties(const ShipClass shipClass) {
		return shipConfig[shipClass];
	}

	inline ShipClass getRandomShipClass() {
		// Generate a random number between 1 and 5
		return static_cast<ShipClass>(vm::randomValue(1, numShipClasses));
	}

	inline std::string shipClassToString(const ShipClass shipClass) {
		switch (shipClass) {
		case ShipClass::Sloop:
			return "Sloop";
		case ShipClass::Brigantine:
			return "Brigantine";
		case ShipClass::Frigate:
			return "Frigate";
		case ShipClass::ManOWar:
			return "Manowar";
		case ShipClass::Galleon:
			return "Galleon";
		default:
			return "Unkown";
		}
	}
}

