#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <tuple>
#include "json.hpp"

#include "ShopItem_PG.h"
#include "PlayerShip_PG.h"

namespace PirateGame {
	namespace JSONSave {
		using json = nlohmann::json;
		static void saveData(const std::string filename, PlayerShip* playerShip) {
			json save;
			save["playerExperience"] = playerShip->getExp();
			save["playerExperienceToLevelUp"] = playerShip->getExpToLevelUp();
			save["playerLevel"] = playerShip->getplayerLevel();
			save["playerGold"] = playerShip->getInventoryHandler()->getGold();
			std::vector<std::tuple<std::string, float, int>> playerInventoryVector;
			for (auto& item : playerShip->getInventoryHandler()->getInventory()) {
				playerInventoryVector.push_back(std::make_tuple(item.name, item.price, item.amount));
			}
			save["playerInventory"] = playerInventoryVector;

			std::ofstream savefile(filename);
			savefile << save.dump(4);
			savefile.close();
		}

	}
}