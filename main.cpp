#include <SFML/Graphics.hpp>
#include <starfield.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800,600,32), "Starfield Example", sf::Style::Default);

    nasic::starfield stars(window, nasic::starfield::starStyle::starsAndPlanets);
    sf::Clock clock;
    sf::Time dt;

    sf::Event e;
    bool running = true;
    while(running)
    {
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }
        }
            dt = clock.restart();

            stars.update(window, dt);
            window.clear();
            window.draw(stars);
            window.display();
    }
    return 0;
}
