#include <SFML/Graphics.hpp>
#include <time.h>

namespace DoodleJump {
    class point { //changed struct to class
    private:
        int x, y;
    public:
        // Setters
        int getX() const { return x; }
        int getY() const { return y; }

        // Setters
        void setX(int newX) { x = newX; }
        void setY(int newY) { y = newY; }
    };

    class World {
    public:
        World(sf::RenderWindow& window) : window(window) {};
        void run() {
            srand(time(0)); //seeding rand
            window.setSize(sf::Vector2u(400, 533)); //setting window size

            sf::Texture t1, t2, t3; //loading the three basic textures
            /*t1.loadFromFile("images/background.png");
            t2.loadFromFile("images/platform.png");
            t3.loadFromFile("images/doodle.png");*/

            sf::Sprite sBackground(t1), sPlat(t2), sPers(t3); //creating sprites

            point plat[10]; // creates platforms for the doodle sprite to jump between
            for (int i = 0; i < 10; i++)
            {
                plat[i].setX(rand() % 400);
                plat[i].setY(rand() % 533);
            }

            int x = 100, y = 100, h = 200; // character variables
            float dx = 0, dy = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x += 3; // registers keystrokes from the player
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x -= 3;

            dy += 0.2; // simulates gravity for the doodle
            y += dy;
            if (y > 500) dy = -10;

            if (y < h) { // loop that lets the doodle interact with platforms
                for (int i = 0; i < 10; i++) {
                    y = h;
                    plat[i].setY(plat[i].getY() - dy);
                    if (plat[i].getY() > 533) {
                        plat[i].setY(0);
                        plat[i].setX(rand() % 400);
                    }
                }
            }

            for (int i = 0; i < 10; i++) { // platform jumping loop
                if ((x + 50 > plat[i].getX()) && (x + 20 < plat[i].getX() + 68)
                    && (y + 70 > plat[i].getY()) && (y + 70 < plat[i].getY() + 14) && (dy > 0)) {
                    dy = -10;
                }
            }

            sPers.setPosition(x, y);

            window.draw(sBackground);
            window.draw(sPers);

            for (int i = 0; i < 10; i++) {
                sPlat.setPosition(plat[i].getX(), plat[i].getY());
                window.draw(sPlat);
            }
        }
    private:
        sf::RenderWindow& window;
    };
}