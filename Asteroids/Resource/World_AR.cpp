#include "World_AR.h"

using namespace Asteriods;

Animation::Animation(Texture& t, int x, int y, int w, int h, int count, float Speed) {
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

void Animation::update() {
    // frames are advanced by the speed
    Frame += speed;
    // # of frames in the animation
    int n = frames.size();
    // wraps to the beginning if necessary
    if (Frame >= n) {
        Frame -= n;
    }
    // setting sprite rectangle
    if (n > 0) {
        sprite.setTextureRect(frames[int(Frame)]);
    }
}

void Entity::settings(Animation& a, int X, int Y, float Angle, int radius) {
	anim = a;
	x = X;
	y = Y;
	angle = Angle;
	R = radius;
}

void Entity::draw(RenderWindow& app) {
	anim.sprite.setPosition(x, y);
	anim.sprite.setRotation(angle + 90);
	app.draw(anim.sprite);

    CircleShape circle(R);
    circle.setFillColor(Color(255, 0, 0, 170));
    circle.setPosition(x, y);
    circle.setOrigin(R, R);
    app.draw(circle);
}

Asteroid::Asteroid() {
	dx = rand() % 8 - 4;
	dy = rand() % 8 - 4;
	name = "asteroid";
    count++;
}

void Asteroid::update() {
	x += dx;
	y += dy;

	if (x > W) {
		x = 0;
	}
	if (x < 0) {
		x = W;
	}
	if (y > H) {
		y = 0;
	}
	if (y < 0) {
		y = H;
	}
}

void Bullet::update() {
	dx = cos(angle * DEGTORAD) * 6;
	dy = sin(angle * DEGTORAD) * 6;
	x += dx;
	y += dy;

	if (x > W || x < 0 || y > H || y < 0) {
		life = false;
	}
}

void Player::update() {
	if (thrust) {
		dx += cos(angle * DEGTORAD) * 0.2;
		dy += sin(angle * DEGTORAD) * 0.2;
	}
	else {
		dx *= 0.99;
		dy *= 0.99;
	}

	int maxSpeed = 15;
	float speed = sqrt(dx * dx + dy * dy);
	if (speed > maxSpeed) {
		dx *= maxSpeed / speed;
		dy *= maxSpeed / speed;
	}

	x += dx;
	y += dy;

	if (x > W) {
		x = 0;
	}
	if (x < 0) {
		x = W;
	}
	if (y > H) {
		y = 0;
	}
	if (y < 0) {
		y = H;
	}
}

UFO::UFO() {
	dx = (rand() % 2 == 0 ? 1 : -1) * 4;
	dy = 0;
	life = true;
	exists = true;
}

void UFO::update() {
	x += dx;
	if (x > W) {
		life = false;
	}
}

World::World(sf::RenderWindow* window) : window(window) {
    // creating main window for the game
    window->setSize(Vector2u(W, H));

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

    // background sprite
    background  = Sprite(t2);

    // declaring animations using loaded textures
    sExplosion = Animation(t3, 0, 0, 256, 256, 48, 0.5);
    sRock = Animation(t4, 0, 0, 64, 64, 16, 0.2);
    sRock_small = Animation(t6, 0, 0, 64, 64, 16, 0.2);
    sBullet = Animation(t5, 0, 0, 32, 64, 16, 0.8);
    sPlayer = Animation(t1, 40, 0, 40, 40, 1, 0);
    sPlayer_go = Animation(t1, 40, 40, 40, 40, 1, 0);
    sUFO = Animation(tUFO, 0, 0, 64, 64, 1, 0);

    // list to store all entities
    std::list<Entity*> entities;

    // creating player entity and adding to entities list
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p);

    // adding 15 asteroids to the entities list
    for (int i = 0; i < 15; i++) {
        Asteroid* a = new Asteroid();
        a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
        entities.push_back(a);
    }
}

void World::run(sf::Event event) {// seeding rand
    // checking for key presses
    if (event.type == Event::KeyPressed)
    {
        // space bar to fire bullet
        if (event.key.code == Keyboard::Space)
        {
            Bullet* b = new Bullet();
            b->settings(sBullet, p->x, p->y, p->angle, 10);
            entities.push_back(b);
        }
        // 'A' key to spawn UFO
        if (event.key.code == Keyboard::A)
        {
            // only spawns if no UFO currently exists
            if (!ufoExists)
            {
                UFO* u = new UFO();
                u->settings(sUFO, 0, 300, 90, 20);
                entities.push_back(u);
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
                        Entity* e = new Asteroid();
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
    for (auto it = entities.begin(); it != entities.end();) {
        Entity* e = *it;

        e->update();
        e->anim.update();

        if (!e->life) {
            it = entities.erase(it);
            delete e;
        }
        else {
            ++it;
        }
    }

    // drawing background and entities
    window->draw(background);
    for (auto e : entities)
    {
        e->draw(*window);
    }
}