#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <cmath>
#include <SFML/Audio.hpp>
#include "iostream"

using namespace sf;

namespace Asteriods {

    // Defining constants for window size
    const int W = 1200;
    const int H = 800;

    // Degrees to radians conversions
    static float DEGTORAD = 0.017453f;

    // static count of existing asteroids for logical use
    static int count;

    // static boolean to determine if UFO exists
    static bool exists = false;

    // Class to manage sprite animations and visual effects
    class Animation {
    public:
        float Frame, speed; // an index for frames and frame speed changing
        Sprite sprite; // sprite animation
        std::vector<IntRect> frames; // texture rectangles for frames

        Animation() {} // Default constructor that initializes empty animation
        Animation(Texture& t, int x, int y, int w, int h, int count, float Speed);

        // updating animation based off of the following frame, using speed to determine.
        void update();

        // checks if the animation cycle completes
        bool isEnd() const {
            // determines if the animation is at or past the final frame
            return Frame + speed >= frames.size();
        }
    };

    // Class for objects with animations and interactive properties
    class Entity {
    public:
        // the position, velocity, radius and angle.
        float x, y, dx, dy, R, angle;
        // entity status, live or dead
        bool life;
        // Identifying the entity name
        std::string name;
        // Animation related to entity
        Animation anim;

        // default constructor where entities start as living.
        Entity() {
            life = true;
        }

        // Entity animation and interactive properties
        void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1);

        // virtual update that gets overridden by the derived classes
        virtual void update() {};

        // draws entities given the current frame of animation
        void draw(RenderWindow& app);

        // virtual destructor to clean up objects once deleted
        virtual ~Entity() {};
    };

    // Asteroid class derived from the entity class for asteroids in game
    class Asteroid : public Entity {
    public:
        // constructor that initializes asteroids w/ random speeds
        Asteroid();

        // destructor that decreases asteroid count when asteroids are destroyed
        virtual ~Asteroid() {
            count--;
        }

        // updates position accounting for wrapping
        void update();
    };


    // bullet class derived from entity, representing projectiles from players
    class Bullet : public Entity {
    public:
        Bullet() {
            name = "bullet";
        }

        // updates bullet position
        void update();
    };

    // Player class derived from entity, represents player
    class Player : public Entity {
    public:
        // determining if thrust is active
        bool thrust;

        Player() {
            name = "player";
        }

        // updating player position using thrust and angular movement
        void update();
    };

    // UFO class derived from entity, adds UFOs alongside asteroids
    class UFO : public Entity {
    public:
        UFO();

        // UFOs become inactive once destroyed
        virtual ~UFO() {
            exists = false;
        }

        // updates UFO position while checking for screen boundaries
        void update();
    };

    // checking for collision based on position and object radius
    inline bool isCollide(Entity* a, Entity* b) {
        return (b->x - a->x) * (b->x - a->x) +
            (b->y - a->y) * (b->y - a->y) <
            (a->R + b->R) * (a->R + b->R);
    }

    // World class
    class World {
    public:
        World(RenderWindow* window);

        void run(sf::Event event);

    private:
        RenderWindow* window;

        // creating player, UFO, and asteroid objects
        Player* p = new Player();
        std::vector<Entity*> entities;

        int asteroidCount = 5;
        int UFOCount = 1;

        bool ufoExists = false;

        // Animations
        Animation sExplosion;
        Animation sRock;
        Animation sRock_small;
        Animation sBullet;
        Animation sPlayer;
        Animation sPlayer_go;
        Animation sUFO;

        sf::Sprite background;
    };
}