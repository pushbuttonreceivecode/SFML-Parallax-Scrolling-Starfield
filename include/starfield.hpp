#ifndef STARFIELD_HPP
#define STARFIELD_HPP

#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <THOR/Math.hpp>

namespace nasic
{
    class starfield : public sf::Drawable
    {
        public:
            starfield(sf::RenderWindow& window, sf::Uint32 style);
            ~starfield();

            void update(sf::RenderWindow& window, sf::Time dt);
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

            enum starStyle
            {
                smallStars,
                allStars,
                starsAndPlanets
            };

        private:
            sf::CircleShape star;
            std::vector<sf::CircleShape> stars;
            static sf::Uint32 m_style;
    };
}

#endif // STARFIELD_HPP
