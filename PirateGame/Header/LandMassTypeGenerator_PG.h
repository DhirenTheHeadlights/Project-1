#pragma once

/// Landmass type generator, so that we can assign a random land mass type to a land mass
/// Also, this way the textures can be synced up with the land mass type in the texture manager

#include "LandMassType_PG.h"
#include "Region_PG.h"
//#include "VectorMath.h"

// This class will assign a random land mass type to a land mass
namespace PirateGame {
    template<typename LMType>
    class LandMassTypeGenerator {
    public:
        explicit LandMassTypeGenerator() {}

        LMType getType(int difficulty = 0) const {
            if constexpr (std::is_same<LMType, IslandType>::value) {
                return getIslandType(difficulty);
            }
            else if constexpr (std::is_same<LMType, RockType>::value) {
                return getRockType(difficulty);
            }
			else if constexpr (std::is_same<LMType, ShipwreckType>::value) {
				return getShipwreckType(difficulty);
			}
        }

    private:
        IslandType getIslandType(int difficulty) const {
            int randomValue = vm::randomValue(difficulty * numIslands, difficulty * numIslands + (numIslands - 1));
            return static_cast<IslandType>(randomValue);
        }

        RockType getRockType(int difficulty) const {
            int randomValue = vm::randomValue(difficulty * numRocks, difficulty * numRocks + (numRocks - 1));
            return static_cast<RockType>(randomValue);
        }

        ShipwreckType getShipwreckType(int difficulty) const {
            int randomValue = vm::randomValue(difficulty * numShipwrecks, difficulty * numShipwrecks + (numShipwrecks - 1));
            return static_cast<ShipwreckType>(randomValue);
        }
    };
}
