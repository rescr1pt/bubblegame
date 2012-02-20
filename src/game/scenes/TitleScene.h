////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_TITLESCENE
#define SIMPLEBUBBLE_TITLESCENE

#include "Scene.h"

class TitleScene final : public Scene
{
    public:
        TitleScene();

        void Update(sf::Clock const& clock) override;
};

#endif
