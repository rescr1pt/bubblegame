////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_COLLISION
#define SIMPLEBUBBLE_COLLISION

namespace sf
{
    class RectangleShape;
    class CircleShape;
}

namespace Collision
{
    bool RectangleTest(sf::Vector2f const& r1Size, sf::Vector2f const& r1Coord,
        sf::Vector2f const& r2Size, sf::Vector2f const& r2Coord);

    bool RectangleTest(const sf::RectangleShape* r1, const sf::RectangleShape* r2);

    bool CircleTest(float c1Radius, sf::Vector2f const& c1coord, float c2Radius,
        sf::Vector2f const& c2coord);

    bool CircleTest(const sf::CircleShape* c1, const sf::CircleShape* c2);

    bool WithInRectangle(sf::Vector2f const& targetCoord, sf::Vector2f const& rSize,
        sf::Vector2f const& rCoord);

    bool WithInCircle(sf::Vector2f const& targetCoord, float cRadius, sf::Vector2f const& cCoord);
}

#endif