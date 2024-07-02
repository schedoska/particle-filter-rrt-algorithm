#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>
#include <numeric>
#include <chrono>
#include <cstdlib>
#include <time.h>

#include "Application.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "My window");
    window.setFramerateLimit(20);
    sf::Clock clock;

    //Scene scene(sf::Vector2f(20, 20));
    //scene.setDrawWalls(false);
    //ControlRobot robot(&scene);
    //robot.setPos(sf::Vector2f(300, 700));
    
    Application application(&window);

    // ------------------------------------------------------------------------------------------

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time dt = clock.restart();

        window.clear(sf::Color::White);

        application.update(dt.asSeconds());
        application.draw();
        
        window.display();
    }

    return 0;
}