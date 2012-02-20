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

#include <TitleScene.h>
#include <PlayScene.h>
#include <Bubble.h>
#include <Game.h>
#include <ResourceMgr.h>


TitleScene::TitleScene()
{
    sf::Vector2u const& winSize = sGame->GetWindow()->getSize();

    // Title sprite
    const sf::Texture* titleTexture = GET_TEXTURE(eResources::Img_Title);
    sf::Sprite* titleSprite = new sf::Sprite(*titleTexture);

    // Set sprite to window size
    titleSprite->setScale(float(winSize.x / titleSprite->getGlobalBounds().width),
        float(winSize.y / titleSprite->getGlobalBounds().height));


    m_drawables.push_back(titleSprite);

    // Title text
    sf::Font const& font = *GET_FONT(eResources::Font_Dejavu);
    sf::Text* m_text = new sf::Text("Tap to play", font, winSize.y / 10);

    // Set text to center
    float textPosX = (winSize.x - m_text->getGlobalBounds().width) / 2;
    float textPosY = (winSize.y - m_text->getGlobalBounds().height) / 2;
    m_text->setPosition(textPosX, textPosY);

    m_drawables.push_back(m_text);
}

void TitleScene::Update(sf::Clock const& clock)
{
    if (sGame->MousePressed())
    {
        sf::Vector2i touchPos = sf::Mouse::getPosition(*sGame->GetWindow());

        if (!ÑheckInScene(1.0f, (sf::Vector2f)touchPos)) {
            return;
        }

        return sGame->SwitchToScene(new PlayScene(touchPos, 1));
    }
}
