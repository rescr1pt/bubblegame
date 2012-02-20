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
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Audio/Sound.hpp>

#include <PlayScene.h>
#include <FinishScene.h>
#include <Util.h>
#include <Collision.h>
#include <ResourceMgr.h>
#include <Game.h>
#include <Bubble.h>

uint16 PlaySceneDefault::PlayerBubbleCount = 1;
uint16 PlaySceneDefault::GoodBubbleCount = 2;
uint16 PlaySceneDefault::BadBubbleCount = 2;
uint16 PlaySceneDefault::FinishMaxLevel = 10;

Score::Score(eScores scoreType, uint32 baseValue = 0) : m_scoreType(scoreType), m_text(nullptr), m_valueText(nullptr)
{
    const uint16 fontSize = sGame->GetWindow()->getSize().x / 40;
    sf::Font const& font = *GET_FONT(eResources::Font_Dejavu);
    std::string textStr = "???";

    switch (scoreType)
    {
        case eScores::GAME_LEVEL:
            textStr = "Game level: ";
            break;
        case eScores::GOOD_BUBBLES:
            textStr = "Good bubbles: ";
            break;
    }

    m_text = new sf::Text(textStr, font, fontSize);
    m_valueText = new sf::Text(std::to_string(baseValue), font, fontSize);
}

Score::~Score()
{
    delete m_text;
    delete m_valueText;
}

void Score::SetValue(uint32 value)
{
    m_valueText->setString(std::to_string(value));
}

uint32 Score::GetValue() const
{
    return std::stol(m_valueText->getString().toAnsiString());
}

void Score::ModifyValue(int32 modCount)
{
    uint32 value = GetValue();
    value += modCount;
    SetValue(value);
}

ScorePanel::ScorePanel()
{
    sf::Vector2u const& windowSize = sGame->GetWindow()->getSize();
    const float cellSize = (float)windowSize.x / 24;

    // Create scores
    const uint16 beginScore = (uint16)eScores::START + 1;
    const uint16 endScore = (uint16)eScores::END;
    for (uint16 i = beginScore; i < endScore; ++i)
    {
        Score* score = new Score((eScores)i, 0);

        // set basic position
        if (i == beginScore)
        {
            const float y = windowSize.y - (cellSize * 1.25f);
            score->GetText()->setPosition(cellSize, y);
        }
        else
        {
            // back text value position 
            Score* prevScore = m_scores.back();
            // set x to backscore + (cell size * 2)
            float posX = prevScore->GetValueText()->getPosition().x + (cellSize * 2);
            float posY = prevScore->GetValueText()->getPosition().y;

            score->GetText()->setPosition(sf::Vector2f(posX, posY));
        }

        CorrectValuePosition(score);
        m_scores.push_back(score);
    }
}

ScorePanel::~ScorePanel()
{
    std::for_each(m_scores.begin(), m_scores.end(), [](Score* s) { delete s; });
    m_scores.clear();
}

Score* ScorePanel::GetScore(eScores scoreType, bool assert) const
{
    ScoreVec::const_iterator it = std::find_if(m_scores.cbegin(), m_scores.cend(), [&](const Score* s)
    {
        return s->GetScoreType() == scoreType;
    });

    if (it != m_scores.end())
        return *it;

    if (assert)
        Assert(true, FS("ScorePanel::GetScore: Can't found score type '%%%'!", (uint32)scoreType));

    return nullptr;
}

void ScorePanel::SetValue(eScores scoreType, uint32 value)
{
    Score* score = GetScore(scoreType, true);
    score->SetValue(value);
    CorrectValuePosition(score);
}

void ScorePanel::ModifyValue(eScores scoreType, int32 modCount)
{
    Score* score = GetScore(scoreType, true);
    score->ModifyValue(modCount);
    CorrectValuePosition(score);
}

void ScorePanel::CorrectValuePosition(Score* score)
{
    const float& textPosX = score->GetText()->getPosition().x;
    const float& textPosY = score->GetText()->getPosition().y;
    const uint16& fontSize = score->GetText()->getCharacterSize();
    const float& textWidth = score->GetText()->getGlobalBounds().width;

    const float& textValuePosX = textPosX + textWidth + (score->GetValue() / fontSize);
    score->GetValueText()->setPosition(textValuePosX, textPosY);
}

void ScorePanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    std::for_each(m_scores.begin(), m_scores.end(), [&](Score* s)
    {
        target.draw(*s->GetText(), states);
        target.draw(*s->GetValueText(), states);
    });
}

PlayScene::PlayScene(sf::Vector2i touchedPosition, uint16 gameLevel)
    : BubbleScript("script_playscene")
    , m_gameLevel(gameLevel)
    , m_bubbleLoadTimer(sGame->GetClock().getElapsedTime().asSeconds())
    , m_isAction(false)
    , m_ActionBeginTimer(m_bubbleLoadTimer)
    , m_bubbleLoaded(false)
    , m_touchPlayerCoordDiff(nullvec2f)
    , m_ptrSoundSprite(nullptr)
    , m_soundEnabled(true)
    , m_scorePanel(nullptr)
{
    m_status = Status::Process;

    const float playerBubbleRadius = PlayerBubbleDefault::MinSize / 2;

    // Set player bubble to center
    const sf::Vector2f centerPlayerCoord(touchedPosition.x - playerBubbleRadius,
        touchedPosition.y - playerBubbleRadius);

    // Add player bubbles
    for (uint16 i = 0; i < PlaySceneDefault::PlayerBubbleCount; ++i)
    {
        // Increment cord
        sf::Vector2f incCoord = (sf::Vector2f)centerPlayerCoord;
        incCoord.x += (i * 2 * PlayerBubbleDefault::MinSize);

        m_bubbles.push_back(new PlayerBubble(playerBubbleRadius, incCoord));
    }

    sf::Texture const& soundTexture = *GET_TEXTURE(eResources::Img_Sound);
    m_ptrSoundSprite = new sf::Sprite(soundTexture);
    m_ptrSoundSprite->setPosition(10, 10);
    m_drawables.push_back(m_ptrSoundSprite);

    m_scorePanel = new ScorePanel;
    m_drawables.push_back(m_scorePanel);

    // Set scores
    m_scorePanel->SetValue(eScores::GAME_LEVEL, (uint32)m_gameLevel);
    m_scorePanel->SetValue(eScores::GOOD_BUBBLES, 0);
}

PlayScene::~PlayScene()
{
    std::for_each(m_bubbles.begin(), m_bubbles.end(), [](Bubble* u) { delete u; });
    m_bubbles.clear();
}

void PlayScene::Update(sf::Clock const& clock)
{
    if (m_status != Status::Process)
    {
        bool win = (m_status == Status::Win);
        return sGame->SwitchToScene(new FinishScene(win, m_gameLevel));
    }

    // Bubbles Loading
    if (!m_bubbleLoaded)
        LoadingBubbles(clock);

    if (!m_isAction)
    {
        if (clock.getElapsedTime().asSeconds() - m_bubbleLoadTimer >= 5)
        {
            m_isAction = true;
        }
    }

    // Bubbles update
    for (BubbleList::iterator it = m_bubbles.begin(); it != m_bubbles.end();)
    {
        Bubble* bubble = (*it);

        // To destroy the burst bubbles.
        if (bubble->IsBurst())
        {
            ++it;
            DestroyBubble(bubble);
            continue;
        }

        // Update acts
        switch (bubble->GetBubbleType())
        {
        case eBubbles::Player: 
            UpdatePlayerAct((PlayerBubble*)bubble, clock); 
            break;
        case eBubbles::Good:
        case eBubbles::Bad: 
            UpdateObstacleAI((ObstacleBubble*)bubble, clock); 
            break;
        }

        bubble->Update(clock);

        ++it;
    }
}

void PlayScene::UpdatePlayerAct(PlayerBubble* bubble, sf::Clock const& clock)
{
    // Reset touching hold

    sf::Vector2f const& mCoord = (sf::Vector2f)sf::Mouse::getPosition(*sGame->GetWindow());

    // On mouse pressed... Hard code!
    if (sGame->MousePressed())
    {
        // On click sound sprite (pause-play music)
        sf::FloatRect const& soundSprRect = m_ptrSoundSprite->getGlobalBounds();
        const sf::Vector2f soundSprSize(soundSprRect.width, soundSprRect.height);

        if (Collision::WithInRectangle(mCoord, soundSprSize, m_ptrSoundSprite->getPosition()))
        {
            m_soundEnabled = !m_soundEnabled;

            if (m_soundEnabled) 
            {
                m_ptrSoundSprite->setColor(sf::Color(sf::Color::White));
                sGame->GetSound()->play();
            }
            else
            {
                m_ptrSoundSprite->setColor(sf::Color(sf::Color::Red));
                sGame->GetSound()->pause();
            }
        }
    }

    const sf::CircleShape* circle = bubble->GetCircle();
    const float bubbleRadius = circle->getRadius();

    // Catch bubble
    if (!bubble->IsCatchedByMouse() && Collision::WithInCircle(mCoord, bubbleRadius, circle->getPosition()))
    {
        bubble->SetCachedByMouse(true);
    }

    if (bubble->IsCatchedByMouse()) {
        // For correct fixation
        m_touchPlayerCoordDiff = mCoord - circle->getPosition();

        const sf::Vector2f fixedCoord(mCoord - m_touchPlayerCoordDiff);

        // Is in scene
        if (ÑheckInScene(bubble->GetCircle()->getRadius(), mCoord))
        {
            bubble->SetToMoveCoord(mCoord);
        }
    }
}

void PlayScene::UpdateObstacleAI(ObstacleBubble* bubble, sf::Clock const& clock)
{
    sf::Vector2u const& winSize = sGame->GetWindow()->getSize();
    const sf::CircleShape* circle = bubble->GetCircle();

    // Side to side
    if (circle->getPosition().x + (circle->getRadius() * 2) >= winSize.x)
        bubble->SetMoveDir(eMoveDirs::Left);
    else if (circle->getPosition().x <= 0)
        bubble->SetMoveDir(eMoveDirs::Right);
}

void PlayScene::LoadingBubbles(sf::Clock const& clock)
{
    const float seconds = clock.getElapsedTime().asSeconds();

    // Create obstacle bubbles (Todo factory)
    if (seconds - m_bubbleLoadTimer >= 3)
    {
        const uint16 bubbleRadius = ObstacleBubbleDefault::Size / 2;
        const uint16 goodBuubleCount = PlaySceneDefault::GoodBubbleCount + (m_gameLevel);
        const uint16 badBuubleCount = PlaySceneDefault::BadBubbleCount + (m_gameLevel + 1);

        sf::Vector2u const& winSize = sGame->GetWindow()->getSize();
        const uint16 maxBubbleCoordCount = winSize.x - (winSize.x - winSize.y);
        const uint16 bubblePlaceSquare = ObstacleBubbleDefault::Size * 4;

        std::vector<sf::Vector2f> possibleCoords;

        // Fill cords
        sf::Vector2u fillVector = winSize.x > winSize.y ? winSize : sf::Vector2u(winSize.y, winSize.x);
        fillVector.x = (uint16)trunc(fillVector.x / bubblePlaceSquare) * bubblePlaceSquare;
        fillVector.y = (uint16)trunc(fillVector.y / bubblePlaceSquare) * bubblePlaceSquare;

        for (uint16 x = bubblePlaceSquare; x < fillVector.x; x += bubblePlaceSquare)
        {
            for (uint16 y = bubblePlaceSquare; y < fillVector.y; y += bubblePlaceSquare)
            {
                possibleCoords.push_back(sf::Vector2f(x, y));
            }
        }

        uint16 obtacleCount = (uint16)(goodBuubleCount + badBuubleCount);

        if (possibleCoords.size() < (uint16)(goodBuubleCount + badBuubleCount))
            Assert(true, FS("PlayScene::LoadingBubbles: Need to generate more coordinates! "
            "Generated coords '%%%' < need count '%%%'.", possibleCoords.size(), obtacleCount));

        // Shuffle
        std::random_shuffle(possibleCoords.begin(), possibleCoords.end());
        uint16 lastBubblePos = 0;
        uint32 bubbleSpeed = (ObstacleBubbleDefault::MoveSpeedMod) + PlaySceneDefault::FinishMaxLevel - m_gameLevel;

        // Creating good bubbles
        for (uint16 i = 0; i < goodBuubleCount; ++i, ++lastBubblePos)
        {
            const eMoveDirs dir = RandI<uint16>(0, 1) ? eMoveDirs::Right : eMoveDirs::Left;
            m_bubbles.push_back(new GoodBubble(bubbleRadius, possibleCoords.at(lastBubblePos), dir, bubbleSpeed));
        }

        // Creating bad bubbles
        for (uint16 i = 0; i < badBuubleCount; ++i, ++lastBubblePos)
        {
            const eMoveDirs dir = RandI<uint16>(0, 1) ? eMoveDirs::Right : eMoveDirs::Left;
            m_bubbles.push_back(new BadBubble(bubbleRadius, possibleCoords.at(lastBubblePos), dir, bubbleSpeed));
        }

        m_scorePanel->SetValue(eScores::GOOD_BUBBLES, goodBuubleCount);

        m_bubbleLoaded = true;
    }
}

Bubble* PlayScene::CheckBubbleCollision(Bubble* bubble) const
{
    BubbleList::const_iterator collisedBubbleIt =
        std::find_if(m_bubbles.cbegin(), m_bubbles.cend(), [&bubble](const Bubble* b)
    {
        return bubble->IsCollision(b);
    });

    if (collisedBubbleIt != m_bubbles.cend())
        return *collisedBubbleIt;

    return nullptr;
}

void PlayScene::OnBubbleMove(Bubble* bubble)
{
    if (!m_isAction)
    {
        return;
    }

    // Check collision
    if (Bubble* collisedBubble = CheckBubbleCollision(bubble))
    {
        bubble->OnTouch(collisedBubble);
        collisedBubble->OnTouch(bubble);
    }
}

Bubble* PlayScene::GetBubble(eBubbles bubbleType) const
{
    BubbleList::const_iterator foundBubble
        = std::find_if(m_bubbles.cbegin(), m_bubbles.cend(), [&bubbleType](const Bubble* b)
    {
        return b->GetBubbleType() == bubbleType;
    });

    return foundBubble != m_bubbles.end() ? (*foundBubble) : nullptr;
}

void PlayScene::DestroyBubble(Bubble* bubble)
{
    if (!bubble)
        return;

    m_bubbles.remove(bubble);

    // Scene ending
    if (bubble->GetBubbleType() == eBubbles::Player)
        m_status = Status::Lose;
    else if (bubble->GetBubbleType() == eBubbles::Good)
    {
        // Set good bubbles score
        const uint32 goodBubbles = std::count_if(m_bubbles.cbegin(), m_bubbles.cend(), [](const Bubble* b)
        {
            return b->GetBubbleType() == eBubbles::Good;
        });
        m_scorePanel->SetValue(eScores::GOOD_BUBBLES, goodBubbles);

        if (!GetBubble(eBubbles::Good))
        {
            Bubble* plBubble = GetBubble(eBubbles::Player);
            Assert(plBubble == nullptr,
                "PlayScene::DestroyBubble: Player bubble not found for finish game");

            m_status = Status::Win;
        }
    }

    delete bubble;
}

void PlayScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Render simple drawables
    Scene::draw(target, states);

    std::for_each(m_bubbles.cbegin(), m_bubbles.cend(), [&](const Bubble* b)
    {
        if (b)
            target.draw(*b, states);
    });
}
