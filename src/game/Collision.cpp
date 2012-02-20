////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <Collision.h>
#include <SFMath.h>

#include <cmath>

namespace Collision
{

bool RectangleTest(sf::Vector2f const& r1Size, sf::Vector2f const& r1Coord,
    sf::Vector2f const& r2Size, sf::Vector2f const& r2Coord)
{
    // Separating Axis Theorem
    return (std::max(r1Coord.x, r2Coord.x) < std::min(r1Coord.x + r1Size.x, r2Coord.x + r2Size.x))
        && (std::max(r1Coord.y, r2Coord.y) < std::min(r1Coord.y + r1Size.y, r2Coord.y + r2Size.y));
}

bool RectangleTest(const sf::RectangleShape* r1, const sf::RectangleShape* r2)
{
    const sf::Vector2f r1Size(r1->getGlobalBounds().height, r1->getGlobalBounds().width);
    const sf::Vector2f r2Size(r2->getGlobalBounds().height, r2->getGlobalBounds().width);

    return RectangleTest(r1Size, r1->getPosition(), r2Size, r2->getPosition());
}

bool CircleTest(float c1Radius, sf::Vector2f const& c1coord, float c2Radius,
    sf::Vector2f const& c2coord)
{
    const sf::Vector2f c1CenterPos = GetCircleCenterPosition(c1Radius, c1coord);
    const sf::Vector2f c2CenterPos = GetCircleCenterPosition(c2Radius, c2coord);

    const float distX = c2CenterPos.x - c1CenterPos.x;
    const float distY = c2CenterPos.y - c1CenterPos.y;

    // Pythagoras theorem
    float dist = std::sqrt((distX * distX) + (distY * distY));

    return dist <= (c1Radius + c2Radius);
}

bool CircleTest(const sf::CircleShape* c1, const sf::CircleShape* c2)
{
    return CircleTest(c1->getRadius(), c1->getPosition(), c2->getRadius(), c2->getPosition());
}

bool WithInRectangle(sf::Vector2f const& targetCoord, sf::Vector2f const& rSize,
    sf::Vector2f const& rCoord)
{
    /*sf::Vector2f rCenterCoord = GetRectangleCenterPosition(rSize, rCoord);

    return (targetCoord.x <= rCenterCoord.x + rSize.x)
    && (targetCoord.x >= rCenterCoord.x - targetCoord.x)
    && (targetCoord.y <= rCenterCoord.y + rSize.y)
    && (targetCoord.y >= rCenterCoord.y - targetCoord.y);
    */

    static const sf::Vector2f targetSize(1, 1);
    return RectangleTest(targetSize, targetCoord, rSize, rCoord);
}

bool WithInCircle(sf::Vector2f const& targetCoord, float cRadius, sf::Vector2f const& cCoord)
{
    static const float targetRadius = 1.f;
    return CircleTest(targetRadius, targetCoord, cRadius, cCoord);
}

}
