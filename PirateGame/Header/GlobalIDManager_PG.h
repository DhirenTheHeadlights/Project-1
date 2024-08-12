#pragma once

/// This class is going to generate all of the specific object IDs in the game.
/// It will be used to generate unique IDs for each object that needs one and 
/// is global to not allow for duplicate IDs. IDs are not stored, just generated.

#include <iostream>

namespace PirateGame {
	class GlobalIDManager {
	public:
		int getUniqueID() {
			return currentID++;
		}
	private:
		int currentID = 0;
	};
}