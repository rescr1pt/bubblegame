////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_BUBBLE_H
#define SIMPLEBUBBLE_BUBBLE_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Clock.hpp>

// Forward decl
namespace sf
{
    class CircleShape;
}

enum class eBubbles : uint16
{
    Player,
    Good,
    Bad
};

enum class eMoveDirs : uint16
{ 
    None,
    Right, 
    Left,
};

sf::Vector2f ConvertDirToCoord(eMoveDirs dir, sf::Vector2f const& interval, sf::Vector2f const& offSet = sf::Vector2f(0, 0));
eMoveDirs ConvertCoordToDir(sf::Vector2f const& coord, sf::Vector2f const& offSet = sf::Vector2f(0, 0));

class Bubble : public sf::Drawable
{
    public:
        explicit Bubble(eBubbles bubbleType, float radius, sf::Vector2f const& coord);
        virtual ~Bubble();

        virtual void Update(sf::Clock const& clock) = 0;
        virtual void OnTouch(Bubble* b) = 0;
        virtual bool IsBurst() const = 0;

        eBubbles GetBubbleType() const { return m_bubbleType; }
        const sf::CircleShape* GetCircle() const { return m_circle; }

        bool IsCollision(const Bubble* b) const;
		
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // final?

    protected:
        eBubbles m_bubbleType;
        sf::CircleShape* m_circle;
};

struct PlayerBubbleDefault
{
	static const float MinSize;
	static const float MaxSize;
	static const float ChangeSizeGood;
    static const float ChangeSizeBad;
};

class PlayerBubble final : public Bubble
{
    public:
        explicit PlayerBubble(float radius, sf::Vector2f const& coord);
        virtual ~PlayerBubble() {}

        void Update(sf::Clock const& clock);
        virtual void OnTouch(Bubble* b) override;
        bool IsBurst() const;

        void SetCachedByMouse(bool catched);
        bool IsCatchedByMouse() const { return m_isCatchedByMouse; }

        void SetToMoveCoord(sf::Vector2f const& toMoveCoord) { m_toMoveCoord = toMoveCoord; }
        // sf::Vector2f const& GetToMoveCoord() const { return m_toMoveCoord; }

    private:
        sf::Vector2f m_toMoveCoord;
        bool m_isCatchedByMouse;
};

struct ObstacleBubbleDefault
{
    static const uint16 Size;
    static const uint16 MoveSpeedMod;
    static const uint16 MinTransparency;
    static const uint16 ChangeTransparency;
};

class ObstacleBubble : public Bubble
{
    public:
        explicit ObstacleBubble(eBubbles bubbleType, float radius,
            sf::Vector2f const& coord, eMoveDirs moveDir, uint32 moveSpeed);

        virtual ~ObstacleBubble() {}

        void Update(sf::Clock const& clock) override;
        virtual void OnTouch(Bubble* b) override;
        bool IsBurst() const override;

        void SetMoveSpeed(uint32 moveSpeed) { m_moveSpeed = moveSpeed; }
        void ModifyMoveSpeed(int32 modCount) { SetMoveSpeed(GetMoveSpeed() + modCount); }
        uint32 GetMoveSpeed() const { return m_moveSpeed; }

        void SetMoveDir(eMoveDirs dir) { m_moveDir = dir; }
        eMoveDirs GetMoveDir() const { return m_moveDir; }
		
    private:
        uint32 m_moveSpeed;
        uint32 m_moveSpeedTimer;
        eMoveDirs m_moveDir;
};

class GoodBubble final: public ObstacleBubble
{
    public:
        explicit GoodBubble(float radius, sf::Vector2f const& coord,
            eMoveDirs moveDir, uint32 moveSpeed);

        virtual ~GoodBubble() {}

        void OnTouch(Bubble* b);
};

class BadBubble final: public ObstacleBubble
{
    public:
        explicit BadBubble(float radius, sf::Vector2f const& coord,
            eMoveDirs moveDir, uint32 moveSpeed);

        virtual ~BadBubble() {}

        void OnTouch(Bubble* b);
};

#endif