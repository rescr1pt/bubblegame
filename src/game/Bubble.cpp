////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Bubble.h>
#include <Collision.h>
#include <ScriptMgr.h>
#include <Game.h>

const float PlayerBubbleDefault::MinSize = 20;
const float PlayerBubbleDefault::MaxSize = 30.0f;
const float PlayerBubbleDefault::ChangeSizeGood = 0.05f;
const float PlayerBubbleDefault::ChangeSizeBad = 0.2f;
const uint16 ObstacleBubbleDefault::Size = 16;
const uint16 ObstacleBubbleDefault::MoveSpeedMod = 12;
const uint16 ObstacleBubbleDefault::MinTransparency = 10;
const uint16 ObstacleBubbleDefault::ChangeTransparency = 1;

sf::Vector2f ConvertDirToCoord(eMoveDirs dir, sf::Vector2f const& interval, sf::Vector2f const& offSet)
{
    sf::Vector2f coord(0, 0);

    switch (dir)
    {
        case eMoveDirs::Right: coord.x = interval.x; break;
        case eMoveDirs::Left: coord.x = -interval.x; break;
        default: coord.x = interval.x; break;
    }

    return coord += offSet;
}

eMoveDirs ConvertCoordToDir(sf::Vector2f const& coord, sf::Vector2f const& offSet)
{
    if (coord.x > offSet.x)      // RIGHT
        return eMoveDirs::Right;
    else if (coord.x < offSet.x) // LEFT
        return eMoveDirs::Left;
    else
        return eMoveDirs::None;
}

Bubble::Bubble(eBubbles bubbleType, float radius, sf::Vector2f const& coord) : m_bubbleType(bubbleType)
{
    m_circle = new sf::CircleShape;

    m_circle->setRadius(radius);
    m_circle->setPosition(coord);

    sScriptMgr->OnBubbleCreate(this);
}

Bubble::~Bubble()
{
    sScriptMgr->OnBubbleDestroy(this);

    delete m_circle;
    m_circle = nullptr;
}

bool Bubble::IsCollision(const Bubble* b) const
{
    return b && b != this && Collision::CircleTest(GetCircle(), b->GetCircle());
}

void Bubble::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_circle)
        target.draw(*m_circle, states);
}

PlayerBubble::PlayerBubble(float radius, sf::Vector2f const& coord)
    : Bubble(eBubbles::Player, radius, coord), m_toMoveCoord(coord), m_isCatchedByMouse(false)
{
    m_circle->setFillColor(sf::Color::White);
}

void PlayerBubble::Update(sf::Clock const& clock)
{
    if (IsBurst())
        return;

    m_circle->setPosition(m_toMoveCoord);
    m_circle->setFillColor(sf::Color::White);

    sScriptMgr->OnBubbleMove(this);
}

void PlayerBubble::OnTouch(Bubble* b)
{
    sf::Color newColor;


    switch (b->GetBubbleType())
    {
        case eBubbles::Good:
        {
            if (m_circle->getRadius() < PlayerBubbleDefault::MaxSize)
                m_circle->setRadius(m_circle->getRadius() + PlayerBubbleDefault::ChangeSizeGood);

            newColor.g = 230;
            break;
        }
        case eBubbles::Bad:
        {
            if (IsBurst())
                break;

            m_circle->setRadius(m_circle->getRadius() - PlayerBubbleDefault::ChangeSizeBad);
            newColor.r = 230;

            break;
        }
    }


    newColor.a = 100;

    m_circle->setFillColor(newColor);
}

bool PlayerBubble::IsBurst() const
{
    return (m_circle->getRadius() * 2 < PlayerBubbleDefault::MinSize);
}

void PlayerBubble::SetCachedByMouse(bool catched)
{
    m_isCatchedByMouse = catched;
    sGame->GetWindow()->setMouseCursorVisible(!m_isCatchedByMouse);
}

ObstacleBubble::ObstacleBubble(eBubbles bubbleType, float radius, sf::Vector2f const& coord, eMoveDirs moveDir, uint32 moveSpeed)
    : Bubble(bubbleType, radius, coord), m_moveSpeed(moveSpeed),
    m_moveDir(moveDir), m_moveSpeedTimer(sGame->GetClock().getElapsedTime().asMilliseconds())
{
}

void ObstacleBubble::Update(sf::Clock const& clock)
{
    if (IsBurst() || GetMoveDir() == eMoveDirs::None)
        return;

    const int32 ms = sGame->GetClock().getElapsedTime().asMilliseconds();
    if (ms - m_moveSpeedTimer >= ObstacleBubbleDefault::MoveSpeedMod)
    {
        sf::Vector2f interval(5, 5);
        m_circle->move(ConvertDirToCoord(GetMoveDir(), interval));
        m_moveSpeedTimer = ms;

        sScriptMgr->OnBubbleMove(this);
    }
}

void ObstacleBubble::OnTouch(Bubble* b)
{
    if (IsBurst())
        return;
}

bool ObstacleBubble::IsBurst() const
{
    return m_circle->getFillColor().a <= ObstacleBubbleDefault::MinTransparency;
}

GoodBubble::GoodBubble(float radius, sf::Vector2f const& coord, eMoveDirs moveDir, uint32 moveSpeed)
    : ObstacleBubble(eBubbles::Good, radius, coord, moveDir, moveSpeed)
{
    // sf::Color::Green
    m_circle->setFillColor(sf::Color(0, 255, 0));
}

void GoodBubble::OnTouch(Bubble* b)
{
    if (IsBurst())
        return;

    if (b->GetBubbleType() == eBubbles::Player)
    {
        sf::Color newColor = m_circle->getFillColor();
        newColor.a -= ObstacleBubbleDefault::ChangeTransparency;

        m_circle->setFillColor(newColor);
    }
}

BadBubble::BadBubble(float radius, sf::Vector2f const& coord, eMoveDirs moveDir, uint32 moveSpeed)
    : ObstacleBubble(eBubbles::Bad, radius, coord, moveDir, moveSpeed)
{
    // sf::Color::Red
    m_circle->setFillColor(sf::Color(255, 0, 0));
}

void BadBubble::OnTouch(Bubble* b)
{
    ObstacleBubble::OnTouch(b);
}
