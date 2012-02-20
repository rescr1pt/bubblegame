////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_FINISHSCENE
#define SIMPLEBUBBLE_FINISHSCENE

#include "Scene.h"

class FinishScene final : public Scene
{
    public:
        explicit FinishScene(bool win, uint16 lastLevel);

        void Update(sf::Clock const& clock) override;

    private:
        uint16 m_lastLevel;
};

#endif
