#include <starfield.hpp>

nasic::starfield::starfield(sf::RenderWindow& window, sf::Uint32 style)
{
    //ctor

    //set up starfield background
    using nasic::starfield;
    m_style = style;
    star.setFillColor(sf::Color(255,255,255,255));

    //set up for placing the stars on a uniform distribution
    std::mt19937 engine;
    std::uniform_int_distribution<int> distr(-100, window.getSize().x);
    auto randomizer = std::bind(distr, engine);
    thor::Distribution<int> thorDistr(randomizer);

    //for calculating a random disturbance factor for offsetting stars
    //this helps to alleviate the clumping behavior seen when
    //the random distribution is less than random >.<
    std::uniform_int_distribution<int> disturb(-50, 50);
    auto randdisturb = std::bind(disturb, engine);
    thor::Distribution<int> thorDisturb(randdisturb);

    //fill the container with stars
    //based on the style passed to the constructor
    std::vector<sf::CircleShape>::iterator starit;

    for(int i=0; i<75; ++i)
    {

        if(m_style == starStyle::allStars || m_style == starStyle::starsAndPlanets)
        {
            float x,y;
            x = thorDistr() + thorDisturb();
            y = thorDistr() + thorDisturb();//addition of disturbance for extra variability

            star.setRadius(5.f);
            star.setFillColor(sf::Color(0, 0, 255, 100));
            star.setPosition(i * star.getGlobalBounds().width + x, i * star.getGlobalBounds().height + y);
            stars.push_back(star);
        }

        for(int j=0; j<75; ++j)
        {
            float x,y;
            x = thorDistr() + thorDisturb();//...again
            y = thorDistr() + thorDisturb();

            if(i == 1 && j == 25)
            {
                if(m_style == starStyle::starsAndPlanets)
                {
                    star.setRadius(50.f);
                    star.setFillColor(sf::Color(149, 69, 53, 255));
                    star.setPosition(i * star.getGlobalBounds().width + x, i * star.getGlobalBounds().height + y);
                    stars.push_back(star);
                }
            }
            else
            {
                star.setRadius(1.f);
                star.setFillColor(sf::Color(255, 255, 255, 255));
                star.setPosition(i * star.getGlobalBounds().width + x, j * star.getGlobalBounds().height + y);
                stars.push_back(star);
            }
        }
    }
}

nasic::starfield::~starfield()
{
    //dtor
}

void nasic::starfield::update(sf::RenderWindow& window, sf::Time dt)
{
    std::vector<sf::CircleShape>::iterator starit;
    for(starit = stars.begin(); starit != stars.end(); ++starit)
    {
        if(starit->getPosition().y < window.getSize().x * 1.2f)
        {
            if(starit->getRadius() == 1.f)
                starit->move(0.f,dt.asSeconds()*5.f);
            else if(starit->getRadius() == 5.f)
                starit->move(0.f,dt.asSeconds()*10.f);
            else if(starit->getRadius() == 50.f)
                starit->move(0.f,dt.asSeconds()*50.f);
        }
        else
        {
            if(starit->getRadius() == 1.f || starit->getRadius() == 5.f)
                starit->setPosition(starit->getPosition().x, 0.f - (window.getSize().y * .1f));

            else if(starit->getRadius() == 50.f)
            {
                //another random generator for resetting position
                //and color of planet
                int randPos = thor::random(-100,(int)window.getSize().x);
                int randR = thor::random(0,255);
                int randG = thor::random(0,255);
                int randB = thor::random(0,255);
                starit->setFillColor(sf::Color(randR,randG,randB,255));
                starit->setPosition(randPos, 0.f - (window.getSize().y * .1f));
            }
        }
    }
}

void nasic::starfield::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //consider an additional star class
    //to keep track of z-indices
    //...I'm of the opinion that in this case
    //that would be unnecessarily complex
    //feel free to improve upon it ;)

    //drawing shapes conditionally via separate loops
    //simulates the effects nicely enough and without fuss
    //because there aren't many variations on this theme

    std::vector<sf::CircleShape>::const_iterator starit;
    //draw small stars first...
    for(starit = stars.begin(); starit != stars.end(); ++starit)
    {
        if(starit->getRadius() == 1.f)
            target.draw(*starit, states);
    }

    //...then the larger stars....
    for(starit = stars.begin(); starit != stars.end(); ++starit)
    {
        if(starit->getRadius() == 5.f)
            target.draw(*starit, states);
    }

    //...and finally, the planets
    for(starit = stars.begin(); starit != stars.end(); ++starit)
    {
        if(starit->getRadius() == 50.f)
            target.draw(*starit, states);
    }
}

//instantiate static members
sf::Uint32 nasic::starfield::m_style = nasic::starfield::starStyle::smallStars;
