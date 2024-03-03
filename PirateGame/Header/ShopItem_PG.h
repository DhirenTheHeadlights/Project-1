#pragma once

/// Simple class to handle the shop items.

#include <string>

namespace PirateGame {
	struct ShopItem {
		std::string name;
		float price;
		int amount;

		ShopItem(const std::string& name, float price, int amount)
			: name(name), price(price), amount(amount) {};

		// Define the equality operator
		bool operator==(const ShopItem& item) const {
			return name == item.name;
		}
	};
}