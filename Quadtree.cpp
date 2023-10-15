#include "QuadTree.h"

bool Boundary::contains(const sf::Vector2f& point) const {
    return (point.x >= (center.x - halfDimension)) &&
        (point.x < (center.x + halfDimension)) &&
        (point.y >= (center.y - halfDimension)) &&
        (point.y < (center.y + halfDimension));
}

bool Boundary::intersects(const Boundary& other) const {
    return !(other.center.x - other.halfDimension > center.x + halfDimension ||
        other.center.x + other.halfDimension < center.x - halfDimension ||
        other.center.y - other.halfDimension > center.y + halfDimension ||
        other.center.y + other.halfDimension < center.y - halfDimension);
}

QuadTree::QuadTree(const Boundary& boundary, int capacity)
    : boundary(boundary), capacity(capacity) {}

bool QuadTree::insert(Pellet* pellet) {
    if (!boundary.contains(pellet->getPosition())) return false;

    if (pellets.size() < capacity) {
        pellets.push_back(pellet);
        return true;
    }
    else {
        if (!divided) {
            subdivide();
        }
        return (northWest->insert(pellet) ||
            northEast->insert(pellet) ||
            southWest->insert(pellet) ||
            southEast->insert(pellet));
    }
}

void QuadTree::query(const Boundary& range, std::vector<Pellet*>& found) const {
    if (!boundary.intersects(range)) return;

    for (Pellet* pellet : pellets) {
        if (range.contains(pellet->getPosition())) {
            found.push_back(pellet);
        }
    }

    if (divided) {
        northWest->query(range, found);
        northEast->query(range, found);
        southWest->query(range, found);
        southEast->query(range, found);
    }
}

void QuadTree::subdivide() {
    float x = boundary.center.x;
    float y = boundary.center.y;
    float h = boundary.halfDimension / 2;

    northWest = std::make_unique<QuadTree>(Boundary(x - h, y - h, h), capacity);
    northEast = std::make_unique<QuadTree>(Boundary(x + h, y - h, h), capacity);
    southWest = std::make_unique<QuadTree>(Boundary(x - h, y + h, h), capacity);
    southEast = std::make_unique<QuadTree>(Boundary(x + h, y + h, h), capacity);
    divided = true;
}

bool QuadTree::isEmpty() const {
    if (!pellets.empty()) return false;
    if (divided) {
        return northWest->isEmpty() && northEast->isEmpty() && southWest->isEmpty() && southEast->isEmpty();
    }
    return true;
}

bool QuadTree::collidesWith(const Circle& circle, Pellet* pellet) const {
    sf::Vector2f circlePos = circle.getPosition();
    sf::Vector2f pelletPos = pellet->getPosition();

    float distance = std::sqrt((circlePos.x - pelletPos.x) * (circlePos.x - pelletPos.x) +
        (circlePos.y - pelletPos.y) * (circlePos.y - pelletPos.y));

    return distance <= circle.getCircleSize();
}

