#pragma once

/// This singleton class contains the game state manager used
/// for controlling which things appear on the screen

namespace PirateGame {
	// Enum class for game states
	enum class GameState {
		Start,
		GameLoop,
		LandMassInteraction,
		OptionsMenu,
		End
	};

	class GlobalGameStateManager {
	public:
		// Public method to access the instance
		static GlobalGameStateManager& getInstance() {
			static GlobalGameStateManager instance;
			return instance;
		}

		// Change the GameState
		void changeGameState(GameState gameState) { 
			this->currGameState = gameState;
			if (gameState == GameState::GameLoop) {
				gameStarted = true;
			}
		}

		// Grab the GameState
		GameState& getCurrentGameState() { return currGameState; }

		// Check if the game has started
		bool hasGameStarted() const { return gameStarted; }
	private:
		// Private Constructor
		GlobalGameStateManager() {};

		// Delete the copy constructor and assignment operator
		GlobalGameStateManager(GlobalGameStateManager const&) = delete;
		GlobalGameStateManager& operator=(GlobalGameStateManager const&) = delete;

		// Variable that stores the GameState
		GameState currGameState = GameState::Start;

		// Track if the game has started
		bool gameStarted = false;
	};
}