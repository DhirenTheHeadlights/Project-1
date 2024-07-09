/* Commenting out for shits and giggles

#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <cmath>
#include <SFML/Audio.hpp>
#include "iostream"

using namespace sf;

// Defining constants for window size
const int W = 1200;
const int H = 800;

// Degrees to radians conversions
float DEGTORAD = 0.017453f;

// Class to manage sprite animations and visual effects
class Animation
{
public:
    float Frame, speed; // an index for frames and frame speed changing
    Sprite sprite; // sprite animation
    std::vector<IntRect> frames; // texture rectangles for frames

    Animation() {} // Default constructor that initializes empty animation

    Animation(Texture& t, int x, int y, int w, int h, int count, float Speed) // constructing animation from textures
    {
        Frame = 0;
        speed = Speed;

        // looping through counts for each frame of animation
        for (int i = 0; i < count; i++)
        {
            frames.push_back(IntRect(x + i * w, y, w, h));
        }

        sprite.setTexture(t);
        // center for rotation
        sprite.setOrigin(w / 2, h / 2);
        // starting at frame 1
        sprite.setTextureRect(frames[0]);
    }

    // updating animation based off of the following frame, using speed to determine.
    void update()
    {
        // frames are advanced by the speed
        Frame += speed;
        // # of frames in the animation
        int n = frames.size();
        // wraps to the beginning if necessary
        if (Frame >= n)
        {
            Frame -= n;
        }
        // setting sprite rectangle
        if (n > 0)
        {
            sprite.setTextureRect(frames[int(Frame)]);
        }
    }

    // checks if the animation cycle completes
    bool isEnd()
    {
        // determines if the animation is at or past the final frame
        return Frame + speed >= frames.size();
    }
};

// Class for objects with animations and interactive properties
class Entity
{
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
    Entity()
    {
        life = true;
    }

    // Entity animation and interactive properties
    void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1)
    {
        anim = a;
        x = X;
        y = Y;
        angle = Angle;
        R = radius;
    }

    // virtual update that gets overridden by the derived classes
    virtual void update() {};

    // draws entities given the current frame of animation
    void draw(RenderWindow& app)
    {
        anim.sprite.setPosition(x, y);
        // adjusts rotation to face upward
        anim.sprite.setRotation(angle + 90);
        // draws the sprite in window
        app.draw(anim.sprite);

        // Circle shape for collision boundary
        CircleShape circle(R);
        // Fills the circle to visualize the hitbox
        circle.setFillColor(Color(255, 0, 0, 170));
        // position of the circle is set to the entity's current position
        circle.setPosition(x, y);
        // rotates around center
        circle.setOrigin(R, R);
        // circle gets drawn in application window
        app.draw(circle);
    }

    // virtual destructor to clean up objects once deleted
    virtual ~Entity() {};
};

// Asteroid class derived from the entity class for asteroids in game
class asteroid : public Entity
{
public:
    // static count of existing asteroids for logical use
    static int count;
    // constructor that initializes asteroids w/ random speeds
    asteroid()
    {
        // random horizontal speed
        dx = rand() % 8 - 4;
        // random vertical speed
        dy = rand() % 8 - 4;
        name = "asteroid";
        count++;
    }

    // destructor that decreases asteroid count when asteroids are destroyed
    virtual ~asteroid()
    {
        count--;
    }

    // updates position accounting for wrapping
    void update()
    {
        x += dx;
        y += dy;

        if (x > W)
        {
            x = 0;
        }
        if (x < 0)
        {
            x = W;
        }
        if (y > H)
        {
            y = 0;
        }
        if (y < 0)
        {
            y = H;
        }
    }
};

// initializing static asteroid count
int asteroid::count = 0;

// bullet class derived from entity, representing projectiles from players
class bullet : public Entity
{
public:
    bullet()
    {
        name = "bullet";
    }

    // updates bullet position
    void update()
    {
        // finding horizontal velocity component
        dx = cos(angle * DEGTORAD) * 6;
        // finding vertical velocity component
        dy = sin(angle * DEGTORAD) * 6;
        // updating position from velocity
        x += dx;
        y += dy;
        // deactivates the bullet if it goes off screen
        if (x > W || x < 0 || y > H || y < 0)
        {
            life = 0;
        }
    }
};

// Player class derived from entity, represents player
class player : public Entity
{
public:
    // determining if thrust is active
    bool thrust;

    player()
    {
        name = "player";
    }

    // updating player position using thrust and angular movement
    void update()
    {
        if (thrust)
        {
            // increasing velocity in angles direction
            dx += cos(angle * DEGTORAD) * 0.2;
            dy += sin(angle * DEGTORAD) * 0.2;
        }
        else
        {
            // simulating drag
            dx *= 0.99;
            dy *= 0.99;
        }
        // setting max speed
        int maxSpeed = 15;
        // computing current speed
        float speed = sqrt(dx * dx + dy * dy);
        // capping max speed
        if (speed > maxSpeed)
        {
            // reducing dy and dx
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }
        // position is updated
        x += dx;
        y += dy;
        // screen-wrapping logic
        if (x > W)
        {
            x = 0;
        }
        if (x < 0)
        {
            x = W;
        }
        if (y > H)
        {
            y = 0;
        }
        if (y < 0)
        {
            y = H;
        }
    }
};

// UFO class derived from entity, adds UFOs alongside asteroids
class UFO : public Entity
{
public:
    // tracking UFO activity
    static bool exists;

    UFO()
    {
        name = "UFO";
        // setting random horizontal UFO movement
        dx = (rand() % 2 == 0 ? 1 : -1) * 4;
        // restricting vertical movement
        dy = 0;
        // UFOs are tracked as being active and alive
        life = true;
        exists = true;
    }

    // UFOs become inactive once destroyed
    virtual ~UFO()
    {
        exists = false;
    }

    // updates UFO position while checking for screen boundaries
    void update()
    {
        // UFO horizontal movement updates
        x += dx;
        // kills UFO if it exits the screen boundaries
        if (x > W || x < 0)
        {
            life = false;
        }
    }
};

// initializing static UFO life
bool UFO::exists = false;

// checking for collision based on position and object radius
bool isCollide(Entity* a, Entity* b)
{
    return (b->x - a->x) * (b->x - a->x) +
        (b->y - a->y) * (b->y - a->y) <
        (a->R + b->R) * (a->R + b->R);
}

// main function
int main()
{
    // seeding rand
    srand(time(0));

    // creating main window for the game
    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);

    // loads textures for in-game entities
    Texture t1, t2, t3, t4, t5, t6, t7, tUFO;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");
    tUFO.loadFromFile("images/ufo.png");
    // <a href="https://www.flaticon.com/free-icons/ufo" title="ufo icons">Ufo
    // icons created by Freepik - Flaticon</a>

    // background sprite
    Sprite background(t2);

    // declaring animations using loaded textures
    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
    Animation sUFO(tUFO, 0, 0, 64, 64, 1, 0);

    // list to store all entities
    std::list<Entity*> entities;

    // creating player entity and adding to entities list
    player* p = new player();
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p);

    // adding 15 asteroids to the entities list
    for (int i = 0; i < 15; i++)
    {
        asteroid* a = new asteroid();
        a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
        entities.push_back(a);
    }

    // the main game loop
    while (app.isOpen())
    {
        // checking for events in the game window
        Event event;
        while (app.pollEvent(event))
        {
            // closing window if necessary
            if (event.type == Event::Closed)
            {
                app.close();
            }
            // checking for key presses
            if (event.type == Event::KeyPressed)
            {
                // space bar to fire bullet
                if (event.key.code == Keyboard::Space)
                {
                    bullet* b = new bullet();
                    b->settings(sBullet, p->x, p->y, p->angle, 10);
                    entities.push_back(b);
                }
                // 'A' key to spawn UFO
                if (event.key.code == Keyboard::A)
                {
                    // only spawns if no UFO currently exists
                    if (!UFO::exists)
                    {
                        UFO* u = new UFO();
                        u->settings(sUFO, 0, 300, 90, 20);
                        entities.push_back(u);
                    }
                }
            }
        }

        // managing player thrust with key inputs
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            p->angle += 3;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            p->angle -= 3;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            p->thrust = true;
        }
        else
        {
            p->thrust = false;
        }

        // collision handling between entities
        for (auto a : entities)
        {
            for (auto b : entities)
            {
                if (a->name == "asteroid" && b->name == "bullet")
                {
                    if (isCollide(a, b))
                    {
                        a->life = false;
                        b->life = false;

                        // splitting asteroids into smaller ones upon collision
                        Entity* e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);

                        // creating two smaller asteroids if the original is large
                        for (int i = 0; i < 2; i++)
                        {
                            if (a->R == 15)
                            {
                                continue;
                            }
                            Entity* e = new asteroid();
                            e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                            entities.push_back(e);
                        }
                    }
                }

                if (a->name == "player" && b->name == "asteroid")
                {
                    if (isCollide(a, b))
                    {
                        b->life = false;

                        // explosion when player hits asteroid
                        Entity* e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);

                        // respawns player at original position
                        p->settings(sPlayer, W / 2, H / 2, 0, 20);
                        p->dx = 0;
                        p->dy = 0;
                    }
                }

                if (a->name == "bullet" && b->name == "UFO")
                {
                    if (isCollide(a, b))
                    {
                        a->life = false;
                        b->life = false;

                        // creating explosion upon UFO destruction
                        Entity* e = new Entity();
                        e->settings(sExplosion, b->x, b->y);
                        e->name = "explosion";
                        entities.push_back(e);
                    }
                }

                if (a->name == "player" && b->name == "UFO")
                {
                    if (isCollide(a, b))
                    {
                        b->life = false;

                        // explosion upon player-UFO collision
                        Entity* e = new Entity();
                        e->settings(sExplosion, b->x, b->y);
                        e->name = "explosion";
                        entities.push_back(e);

                        // respawns player at original position
                        p->settings(sPlayer, W / 2, H / 2, 0, 20);
                        p->dx = 0;
                        p->dy = 0;
                    }
                }
            }
        }

        // updating entity states and removing dead ones
        for (auto it = entities.begin(); it != entities.end();)
        {
            Entity* e = *it;

            e->update();
            e->anim.update();

            if (!e->life)
            {
                it = entities.erase(it);
                delete e;
            }
            else
            {
                ++it;
            }
        }

        // drawing background and entities
        app.draw(background);
        for (auto e : entities)
        {
            e->draw(app);
        }
        app.display();
    }

    return 0;
}

*/