#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>

using namespace sf;

int klicker();

bool active = true;


int main()
{
    std::vector<std::thread> threads;

    auto window = sf::RenderWindow(sf::VideoMode({500, 500}), "Menu");
    window.setFramerateLimit(144);


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                active = false;
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Space))
        {   
            threads.push_back(std::thread(klicker));
            sf::sleep(sf::milliseconds(150));
        }

        window.clear();
        window.display();
    }

    //Es wird gewartet bis alle threads beendet sind
    for (auto& th : threads) {
        if (th.joinable())
        {
            th.join();
        }
    }
}

int klicker() 
{
    auto window = sf::RenderWindow(sf::VideoMode({500, 500}), "Klicker");
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        if (active == false) 
        {
            window.close();
        }

        window.clear();
        window.display();
    }

    return 0;
}