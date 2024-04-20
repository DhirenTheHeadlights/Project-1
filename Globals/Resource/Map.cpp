#include "Map.h"

// Constructor
void Map::grid(int boardLength, int boardHeight, int cellSize, sf::Vector2f position) {
    rows = boardHeight / cellSize;
    cols = boardLength / cellSize;

    // Set variables
    this->cellSize = cellSize;
    this->len = boardLength;
    this->height = boardHeight;

    // Set the position of the map
    this->position = position;
}

// This function uses a poisson disc sampling algorithm to generate a set of points
// that are at least a certain distance apart from each other. This function should be
// called once to generate the points.
std::vector<sf::Vector2f> const Map::getRandomPositions(float minDistance, int numPoints) const {
    const int k = 30; // attempts before giving up on finding a new point
    const float pi = 3.14159265358979323846f;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    std::vector<sf::Vector2f> samplePoints;
    std::vector<sf::Vector2f> activeList;

    // Initialize with the first point
    sf::Vector2f initialPoint(dis(gen) * static_cast<float>(len), dis(gen) * static_cast<float>(height));
    initialPoint += position; // Adjust for the position of the map
    activeList.push_back(initialPoint);
    samplePoints.push_back(initialPoint);

    while (!activeList.empty() && samplePoints.size() < static_cast<size_t>(numPoints)) {
        std::uniform_int_distribution<> dist(0, static_cast<int>(activeList.size()) - 1);
        int randIndex = dist(gen);
        sf::Vector2f point = activeList[randIndex];
        bool found = false;

        for (int i = 0; i < k && samplePoints.size() < static_cast<size_t>(numPoints); ++i) {
            float angle = dis(gen) * 2 * pi;
            float radius = minDistance * (1 + dis(gen));
            // Generate a new point, this doesnt need to be adjusted for the map position
            // because the point is generated relative to the current point, which is already adjusted
            sf::Vector2f newPoint(point.x + radius * cos(angle), point.y + radius * sin(angle));

            // Check bounds and minimum distance
            sf::Vector2f bounds = position + sf::Vector2f(static_cast<float>(len), static_cast<float>(height));
            if (newPoint.x >= position.x && newPoint.x < bounds.x && newPoint.y >= position.y && newPoint.y < bounds.y) {
                bool tooClose = false;
                for (const sf::Vector2f& existingPoint : samplePoints) {
                    float distance = std::hypot(newPoint.x - existingPoint.x, newPoint.y - existingPoint.y);
                    if (distance < minDistance) {
                        tooClose = true;
                        break;
                    }
                }
                if (!tooClose) {
                    activeList.push_back(newPoint);
                    samplePoints.push_back(newPoint);
                    found = true;
                    if (samplePoints.size() >= static_cast<size_t>(numPoints)) {
                        break; // Exit if we have reached the desired number of points
                    }
                }
            }
        }

        if (!found) {
            // If no point is found after k attempts, remove the current point from the active list
            activeList.erase(activeList.begin() + randIndex);
        }
    }

    return samplePoints;
}

// Draw the grid
void Map::drawGrid(sf::RenderWindow& window) const {
    sf::RectangleShape line(sf::Vector2f(static_cast<float>(len), 1.f)); // Horizontal line, length adjusted if needed
    line.setFillColor(sf::Color::Red);
    for (int i = 0; i <= rows; i++) {
        line.setPosition(position.x, position.y + static_cast<float>(i * cellSize));
        window.draw(line);
    }
    line.setSize(sf::Vector2f(1.f, static_cast<float>(height))); // Adjust line size for vertical lines, if necessary
    for (int i = 0; i <= cols; i++) {
        line.setPosition(position.x + static_cast<float>(i * cellSize), position.y);
        window.draw(line);
    }
}
