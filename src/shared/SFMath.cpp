////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <SFMath.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

sf::Vector2f GetRectangleCenterPosition(sf::Vector2f const& rSize, sf::Vector2f const& rCoord)
{
    sf::Vector2f centerPos;
    centerPos.x = rCoord.x + (rSize.x / 2.f);
    centerPos.y = rCoord.y + (rSize.y / 2.f);

    return centerPos;
}

sf::Vector2f GetRectangleCenterPosition(const sf::RectangleShape* rectangle)
{
    const sf::Vector2f size(rectangle->getGlobalBounds().width, rectangle->getGlobalBounds().height);
    return GetRectangleCenterPosition(size, rectangle->getPosition());
}

sf::Vector2f GetCircleCenterPosition(float cRadius, sf::Vector2f const& rCoord)
{
    sf::Vector2f centerPos;
    centerPos.x = rCoord.x + cRadius;
    centerPos.y = rCoord.y + cRadius;

    return centerPos;
}

sf::Vector2f GetCircleCenterPosition(const sf::CircleShape* circle)
{
    return GetCircleCenterPosition(circle->getRadius(), circle->getPosition());
}