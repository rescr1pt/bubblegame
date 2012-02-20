////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_SCENE_H
#define SIMPLEBUBBLE_SCENE_H

#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Clock.hpp>

class Scene : public sf::Drawable
{
    public:
        typedef std::vector<sf::Drawable*> DrawableVec;
        Scene();
        virtual ~Scene();

        virtual void Update(sf::Clock const& clock) = 0;

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        bool ÑheckInScene(float bRadius, sf::Vector2f const& bCoord) const;

    protected:
        DrawableVec m_drawables;
};

#endif