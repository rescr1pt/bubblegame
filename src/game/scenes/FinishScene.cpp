////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Mouse.hpp>

#include <FinishScene.h>
#include <PlayScene.h>
#include "TitleScene.h"
#include <Game.h>
#include <ResourceMgr.h>
#include <Bubble.h>

FinishScene::FinishScene(bool win, uint16 lastLevel)
    : m_lastLevel(lastLevel)
{
    sf::Vector2u const& winSize = sGame->GetWindow()->getSize();

    // Finish text
    sf::Font const& font = *GET_FONT(eResources::Font_Dejavu);
    sf::Text* m_text = new sf::Text;
    m_text->setFont(font);
    m_text->setCharacterSize(winSize.x / 20);

    std::string finishText;

    if (win)
    {
        if (lastLevel < PlaySceneDefault::FinishMaxLevel) {
            finishText = "You won! Tap to go to level ";
            finishText += std::to_string(m_lastLevel + 1) + ".";
        }
        else
        {
            finishText = "You won! Tap to restart the game.";

        }
    }
    else {
        finishText = "Game over!";
    }

    m_text->setString(finishText);

    // Set text to center
    const float textPosX = (winSize.x - m_text->getGlobalBounds().width) / 2;
    const float textPosY = (winSize.y - m_text->getGlobalBounds().height) / 2;
    m_text->setPosition(textPosX, textPosY);

    m_drawables.push_back(m_text);
}

void FinishScene::Update(sf::Clock const& clock)
{
    if (sGame->MousePressed())
    {
        sf::Vector2i touchPos = sf::Mouse::getPosition(*sGame->GetWindow());

        if (!ÑheckInScene(1.0f, (sf::Vector2f)touchPos)) {
            return;
        }
        return m_lastLevel < PlaySceneDefault::FinishMaxLevel
            ? sGame->SwitchToScene(new PlayScene(touchPos, m_lastLevel + 1))
            : sGame->SwitchToScene(new TitleScene);
    }
}
