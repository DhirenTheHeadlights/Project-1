#pragma once

/// This class is going to generate all of the specific object IDs in the game.
/// It will be used to generate unique IDs for each object that needs one and 
/// is global to not allow for duplicate IDs. IDs are not stored, just generated.

#include <iostream>

namespace PirateGame {
	class GlobalIDManager {
	public:
		static GlobalIDManager& getInstance() {
			static GlobalIDManager instance;
			return instance;
		}

		int getUniqueID() {
			return currentID++;
		}

	private:
		GlobalIDManager() {};
		~GlobalIDManager() {};

		GlobalIDManager(GlobalIDManager const&) = delete;
		GlobalIDManager& operator=(GlobalIDManager const&) = delete;

		int currentID = 0;
	};
}