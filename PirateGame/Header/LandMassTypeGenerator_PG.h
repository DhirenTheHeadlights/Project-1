#pragma once

/// Landmass type generator, so that we can assign a random land mass type to a land mass
/// Also, this way the textures can be synced up with the land mass type in the texture manager

#include <random>
#include "LandMassType_PG.h"

// This class will assign a random land mass type to a land mass
namespace PirateGame {
    template<typename LMType>
    class LandMassTypeGenerator {
    public:
        explicit LandMassTypeGenerator() {}

        LMType getType() const {
            if constexpr (std::is_same<LMType, IslandType>::value) {
                return getIslandType();
            }
            else if constexpr (std::is_same<LMType, RockType>::value) {
                return getRockType();
            }
			else if constexpr (std::is_same<LMType, ShipwreckType>::value) {
				return getShipwreckType();
			}
        }

    private:
        IslandType getIslandType() const {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dis(0, 1); // Assuming 2 island types
            return static_cast<IslandType>(dis(gen));
        }

        RockType getRockType() const {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dis(0, 8); // Assuming 9 rock types
            return static_cast<RockType>(dis(gen));
        }
        ShipwreckType getShipwreckType() const {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dis(0, 0); // Assuming 1 wreck type
            return static_cast<ShipwreckType>(dis(gen));
        }
    };
}
