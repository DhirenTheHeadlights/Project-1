#include "WorldLS.h"

using namespace LightAndShadow;

World::World(sf::RenderWindow& window) : window(window) {
	initializeGameEntities();
}

void World::createWorld(sf::Event event) {
	update();
	render();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}
}

void World::update() {
	// Center the emitter in the window
	float x = window.getSize().x / 2 - lightEmitter.getRadius();
	float y = window.getSize().y / 2 - lightEmitter.getRadius();
	lightEmitter.setPosition(x, y);

	// Create a convex shape for the dynamic 'light' part
	lightShape.setPointCount(3); // triangle for the light cone

	// The base of the triangle will be the width of the window
	lightShape.setPoint(0, sf::Vector2f(0, window.getSize().y));
	lightShape.setPoint(1, sf::Vector2f(window.getSize().x, window.getSize().y));

	// The top of the triangle is the position of the emitter
	lightShape.setPoint(2, sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	lightShape.setFillColor(sf::Color(255, 255, 0, 100)); // semi-transparent yellow

	// This code should also move the player around the screen with WASD
	float playerSpeed = 100; // adjust as needed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player.move(0, -playerSpeed * mainClock.restart().asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player.move(0, playerSpeed * mainClock.restart().asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.move(-playerSpeed * mainClock.restart().asSeconds(), 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.move(playerSpeed * mainClock.restart().asSeconds(), 0);
	}

}


void World::render() {
	window.draw(background);
	window.draw(lightEmitter);
	window.draw(lightShape);
	window.draw(player);
	for (auto& obstacle : obstacles) {
		window.draw(obstacle);
	}
	for (auto& obstacle : obstacles) {

		std::vector<sf::Vector2f> corners = {
		obstacle.getPosition(),
		obstacle.getPosition() + sf::Vector2f(obstacle.getSize().x, 0),
		obstacle.getPosition() + obstacle.getSize(),
		obstacle.getPosition() + sf::Vector2f(0, obstacle.getSize().y)
		};
		bool inLight = false;
		for (const auto& corner : corners) {
			if (lightShape.getGlobalBounds().contains(corner)) { inLight = true; }
		}
		if (inLight) {
			float x = window.getSize().x / 2;
			float y = window.getSize().y / 2;
			auto shadowPoints = calculateShadow(obstacle, sf::Vector2f(x, y));
			auto shadowShape = createShadowShape(shadowPoints);
			window.draw(shadowShape);
		}

	}
	for (auto& obstacle : obstacles) {
		window.draw(obstacle);
	}
}

void World::initializeGameEntities() {

	background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	background.setFillColor(sf::Color(50, 50, 50)); // Dark gray

	lightEmitter.setRadius(lightEmitterSize);
	lightEmitter.setFillColor(sf::Color::Black);
	lightEmitter.setPosition(window.getSize().x / 2 - lightEmitter.getRadius(), window.getSize().y / 2 - lightEmitter.getRadius());

	player.setPointCount(4);
	player.setFillColor(sf::Color::White);

	generateObstacles();
}

void World::generateObstacles() {

	int numObstacles = std::rand() % 2 + 6;
	float minDistanceFromWall = playerSize;
	float minDistanceFromEmitter = lightEmitter.getRadius() * 3;

	for (int i = 0; i < numObstacles; ++i) {
		sf::RectangleShape obstacle(sf::Vector2f(obstacleSize, obstacleSize));
		obstacle.setFillColor(sf::Color::White);

		sf::Vector2f position;
		bool validPosition = false;
		while (!validPosition) {
			position.x = std::rand() % (static_cast<int>(window.getSize().x) - static_cast<int>(minDistanceFromWall * 2)) + minDistanceFromWall;
			position.y = std::rand() % (static_cast<int>(window.getSize().y) - static_cast<int>(minDistanceFromWall * 2)) + minDistanceFromWall;

			validPosition = true;

			if (sqrt(pow(position.x - lightEmitter.getPosition().x, 2) + pow(position.y - lightEmitter.getPosition().y, 2)) < minDistanceFromEmitter) {
				validPosition = false;
			}

			for (auto& other : obstacles) {
				if (sqrt(pow(position.x - other.getPosition().x, 2) + pow(position.y - other.getPosition().y, 2)) < obstacleSize * 2) {
					validPosition = false;
					break;
				}
			}
		}

		obstacle.setPosition(position);
		obstacles.push_back(obstacle);
	}
}

// Function to calculate the shadow polygon for a single rectangle
std::vector<sf::Vector2f> World::calculateShadow(const sf::RectangleShape& box, const sf::Vector2f& lightPos) {
	std::vector<sf::Vector2f> cornerPoints;
	std::vector<sf::Vector2f> castDirections;

	// Get the position of the box and its size
	sf::Vector2f boxPos = box.getPosition();
	sf::Vector2f boxSize = box.getSize();

	// Calculate the corners of the box
	std::vector<sf::Vector2f> corners = {
		boxPos,
		boxPos + sf::Vector2f(boxSize.x, 0),
		boxPos + boxSize,
		boxPos + sf::Vector2f(0, boxSize.y)
	};


	// Calculate shadow points for each corner
	for (const auto& corner : corners) {
		sf::Vector2f dir = corner - lightPos;
		float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		// Make the shadow length proportional to the distance from the light

		if (!box.getGlobalBounds().contains(corner + (dir / length))) {
			dir.x *= 100; // Length of the shadow, should be long enough to go off-screen
			dir.y *= 100;
			cornerPoints.push_back(corner);
			castDirections.push_back(dir);
			std::cout << dir.x << " " << dir.y << std::endl;
		}

	}


	std::vector<sf::Vector2f> shadowCasts{};
	for (int i{ 0 }; i < cornerPoints.size(); i++) {
		shadowCasts.push_back(cornerPoints[i] + castDirections[i]);
	}
	std::vector<sf::Vector2f> shadowPoints{};
	for (const auto& corner : cornerPoints) {
		shadowPoints.push_back(corner);
	}
	for (const auto& shadowVertex : shadowCasts) {
		shadowPoints.push_back(shadowVertex);
	}


	return shadowPoints;
}

// Function to create a shadow shape from points
sf::ConvexShape World::createShadowShape(const std::vector<sf::Vector2f>& points) {
	sf::ConvexShape shadow;
	shadow.setPointCount(points.size());
	for (size_t i = 0; i < points.size(); ++i) {
		shadow.setPoint(i, points[i]);
	}
	shadow.setFillColor(sf::Color(50, 50, 50)); // Semi-transparent black
	//shadow.setFillColor(sf::Color::Magenta);
	return shadow;
}