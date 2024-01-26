#pragma once
#include "Pellet.h"
#include "Circle.h"
#include <vector>
#include <memory>

struct Boundary {
    sf::Vector2f center;
    float halfDimension;

    Boundary(float x, float y, float halfDimension)
        : center(x, y), halfDimension(halfDimension) {}

    bool contains(const sf::Vector2f& point) const;
    bool intersects(const Boundary& range) const;
};

class QuadTree {
public:
    QuadTree(const Boundary& boundary, int capacity);

    bool insert(Pellet* pellet);
    bool isEmpty() const;
    bool collidesWith(const Circle& circle, Pellet* pellet) const;
    void query(const Boundary& range, std::vector<Pellet*>& found) const;

private:
    Boundary boundary;
    std::vector<Pellet*> pellets;
    const int capacity;
    bool divided = false;

    std::unique_ptr<QuadTree> northWest;
    std::unique_ptr<QuadTree> northEast;
    std::unique_ptr<QuadTree> southWest;
    std::unique_ptr<QuadTree> southEast;

    void subdivide();
};
