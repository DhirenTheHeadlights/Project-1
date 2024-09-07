#pragma once

/// Landmass type generator, so that we can assign a random land mass type to a land mass
/// Also, this way the textures can be synced up with the land mass type in the texture manager

#include "LandMassType_PG.h"
//#include "VectorMath.h"

// This class will assign a random land mass type to a land mass
namespace PirateGame {
    template<typename LmType>
    class LandMassTypeGenerator {
    public:
        explicit LandMassTypeGenerator() = default;

        [[nodiscard]] LmType getType(const int difficulty = 0) const {
            if constexpr (std::is_same_v<LmType, IslandType>) {
                return getIslandType(difficulty);
            }
            else if constexpr (std::is_same_v<LmType, RockType>) {
                return getRockType(difficulty);
            }
			else if constexpr (std::is_same_v<LmType, ShipwreckType>) {
				return getShipwreckType(difficulty);
			}
            return LmType();
        }

    private:
        static IslandType getIslandType(const int difficulty) {
            int randomValue = vm::randomValue(difficulty * numIslands, difficulty * numIslands + (numIslands - 1));
            return static_cast<IslandType>(randomValue);
        }

        static RockType getRockType(const int difficulty) {
            int randomValue = vm::randomValue(difficulty * numRocks, difficulty * numRocks + (numRocks - 1));
            return static_cast<RockType>(randomValue);
        }

        static ShipwreckType getShipwreckType(const int difficulty) {
            int randomValue = vm::randomValue(difficulty * numShipwrecks, difficulty * numShipwrecks + (numShipwrecks - 1));
            return static_cast<ShipwreckType>(randomValue);
        }
    };
}
