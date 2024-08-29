#pragma once
#include <SFML/Graphics.hpp>

#include "VectorMath.h"
#include "QuadtreeTemplate_PG.h"
#include "JSONLoader_PG.h"
#include "Cannonball_PG.h"
#include "ShipEnums_PG.h"


namespace PirateGame {
	// Enum for firing state
	enum class FiringState {
		TowardsTarget,
		TowardsMouse,
		Untargeted
	};

	class ShipCannon {
	public:
		ShipCannon(JSONLoader* jsl, const sf::Texture& texture, ID* id, FiringSide FS, sf::Vector2f scale) : jsl(jsl), cannonSprite(texture), id(id), side(FS) {
			cannonSprite.setScale(scale);

			if (FS == FiringSide::Port) {
				defaultRotation = 180.f;
				// The origin is not perfectly in the center of the cannon
				// so we need to adjust it slightly (the cannon sprite has
				// an even number of pixels in width and height)
				cannonSprite.setOrigin(7.f, 7.f); 
			}
			else {
				defaultRotation = 0.f;
				cannonSprite.setOrigin(5.f, 5.f);
			}
		};
		~ShipCannon() {};

		void fireCannon(FiringSide FS, const sf::Sprite& shipSprite, const sf::Texture& cannonballTexture, GlobalIDManager* GIDM);
		void drawCannonNBalls(sf::RenderWindow* window);
		void updateCannon(const sf::Sprite& shipSprite, FiringSide FS, sf::RenderWindow* window, sf::Time elapsed);

		// Setters
		void setCannonballHashmap(Quadtree<Cannonball>* cannonballHashmap) { this->cannonballHashmap = cannonballHashmap; }
		void setOffset(sf::Vector2f offset) { this->offset = offset; }
		void setFiringState(FiringState FS) { this->state = FS; }
		void setTargetPos(sf::Vector2f targetPos) { this->targetPos = targetPos; }
	
		// Getters
		sf::Sprite& getSprite() { return cannonSprite; }
		sf::Vector2f getOffset() const { return offset; }
		float getFiringDirectionAngle() const { return firingDirectionAngle; }
		FiringSide getFiringSide() const { return side; }
	private:
		JSONLoader* jsl;

		// Helpers
		sf::Vector2f calculateDirectionToTarget(const sf::Sprite& shipSprite, sf::Vector2f targetPos);
		sf::Vector2f calculatePerpendicularDirection(float rotation) const;
		void rotateTowards(float angle, float step);
		void updateCannonRotation(const sf::Sprite& shipSprite, FiringSide FS, sf::RenderWindow* window);
		void updateCannonballs(sf::Time elapsed);

		sf::Clock resetRotationClock;

		sf::Sprite cannonSprite;
		std::vector<Cannonball*> cannonballs;
		sf::Vector2f offset;
		sf::Vector2f targetPos;
		sf::Vector2f fireDirection;

		const float approxCannonOffsetToEdgeRatio = 0.85f;
		float defaultRotation = 0.f;
		float firingDirectionAngle = 0.f;
		float aimRotation = 0.f;

		FiringSide side = FiringSide::Starboard;
		FiringState state = FiringState::Untargeted;

		Quadtree<Cannonball>* cannonballHashmap = nullptr;

		ID* id;
	};
}
	