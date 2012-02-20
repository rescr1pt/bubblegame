////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_SFMATH
#define SIMPLEBUBBLE_SFMATH

namespace sf
{
    class RectangleShape;
    class CircleShape;
}

sf::Vector2f GetRectangleCenterPosition(sf::Vector2f const& rSize, sf::Vector2f const& rCoord);
sf::Vector2f GetRectangleCenterPosition(const sf::RectangleShape* rectangle);
sf::Vector2f GetCircleCenterPosition(float cRadius, sf::Vector2f const& rCoord);
sf::Vector2f GetCircleCenterPosition(const sf::CircleShape* circle);

#endif