#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>


int main() {

    sf::RenderWindow window(sf::VideoMode(2560, 1440), "RANGER WINDOW");
    sf::CircleShape shape(300.f);
    int originx{ 1280 };
    int originy{ 720 };
    shape.setPosition(originx, originy);
    shape.setFillColor(sf::Color::Blue);
    int x{ originx };
    int y{ originy };

    
    sf::Clock clock;
    while (window.isOpen()){
        
        sf::Event event;
        while (window.pollEvent(event)){
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Left)
                    x -= 50;
                else if (event.key.code == sf::Keyboard::Right)
                    x += 50;
                else if (event.key.code == sf::Keyboard::Up)
                    y -= 50;
                else if (event.key.code == sf::Keyboard::Down)
                    y += 50;
                break;
            }
        }

        window.clear();
        window.draw(shape);
        shape.setPosition(float(x), float(y));
        window.display();
        
    }

    return 0;
}




/*
int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Blue);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
*/


/*int thread_iteration(int x) {
    std::cout << "This is thread 1, iteration: " << x << std::endl;
    return 0;

}
int thread_print() {
    std::cout << "Thread 2" << std::endl;
    return 0;
}
*/
/*    int count{0};
    while (1 == 1) {
        std::cout << "This is a thread test.\n";
        std::thread first(thread_iteration, count);
        std::thread second(thread_print);
        first.join();
        second.join();
        count++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
*/