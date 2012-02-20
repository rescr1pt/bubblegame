////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <SFML/Graphics/RenderWindow.hpp>

#include <Scene.h>
#include <Game.h>

Scene::Scene() 
{
}

Scene::~Scene()
{
    std::for_each(m_drawables.begin(), m_drawables.end(), [](sf::Drawable* d)
    {
        delete d;
    });
    m_drawables.clear();
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    std::for_each(m_drawables.cbegin(), m_drawables.cend(), [&](const sf::Drawable* d)
    {
        if (d)
            target.draw(*d, states);
    });
}

bool Scene::ÑheckInScene(float bRadius, sf::Vector2f const& bCoord) const
{
    sf::Vector2u const& winSize = sGame->GetWindow()->getSize();
    const float diam = bRadius * 2;

    return bCoord.x >= 0 && (bCoord.x + diam) <= winSize.x
        && bCoord.y >= 0 && (bCoord.y + diam) <= winSize.y;
}
