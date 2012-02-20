////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_PLAYSCENE
#define SIMPLEBUBBLE_PLAYSCENE

#include <list>

#include "Scene.h"
#include "ScriptMgr.h"

class Bubble;
class PlayerBubble;
class ObstacleBubble;

enum class eBubbles : uint16;

namespace sf
{
    class Text;
    class Sprite;
}

enum class eScores : int
{
    START,
    GAME_LEVEL,
    GOOD_BUBBLES,
    END
};

struct PlaySceneDefault
{
    static uint16 PlayerBubbleCount;
    static uint16 GoodBubbleCount;
    static uint16 BadBubbleCount;
    static uint16 FinishMaxLevel;
};

class Score
{
    public:
        explicit Score(eScores scoreType, uint32 baseValue);
        ~Score();

        void SetValue(uint32 value);
        void ModifyValue(int32 value);
        uint32 GetValue() const;

        sf::Text* GetText() { return m_text; }
        sf::Text* GetValueText() { return m_valueText; }
        eScores GetScoreType() const { return m_scoreType; }

    private:
        sf::Text* m_text;
        sf::Text* m_valueText;
        eScores m_scoreType;
};

class ScorePanel final : public sf::Drawable
{
    public:
        typedef std::vector<Score*> ScoreVec;

        ScorePanel();
        virtual ~ScorePanel();

        Score* GetScore(eScores scoreType, bool assert = false) const;
        uint32 GetValue(eScores scoreType) const { return GetScore(scoreType, true)->GetValue(); }
        void SetValue(eScores scoreType, uint32 value);
        void ModifyValue(eScores scoreType, int32 modCount);

    private:
        void CorrectValuePosition(Score* score);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        ScoreVec m_scores;
};

class PlayScene final : public Scene, protected BubbleScript
{
    public:
        typedef std::list<Bubble*> BubbleList;

        // Todo memento pattern?
        explicit PlayScene(sf::Vector2i touchedPosition, uint16 gameLevel);
        virtual ~PlayScene();

        void Update(sf::Clock const& clock) override;
        void UpdatePlayerAct(PlayerBubble* bubble, sf::Clock const& clock);
        void UpdateObstacleAI(ObstacleBubble* bubble, sf::Clock const& clock);

        Bubble* GetBubble(eBubbles bubbleType) const;
        void DestroyBubble(Bubble* bubble);
        Bubble* CheckBubbleCollision(Bubble* bubble) const;

    private:
        void LoadingBubbles(sf::Clock const& clock);

        // Script override
        void OnBubbleMove(Bubble* bubble) override;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        enum class Status : uint16
        {
            Process,
            Win,
            Lose
        } m_status;

    private:
        uint16 m_gameLevel;
        BubbleList m_bubbles;
        bool m_bubbleLoaded;
        float m_bubbleLoadTimer;

        bool m_isAction;
        float m_ActionBeginTimer;

        sf::Vector2f m_touchPlayerCoordDiff;

        // Pointer to sound sprite
        sf::Sprite* m_ptrSoundSprite;
        bool m_soundEnabled;

        ScorePanel* m_scorePanel;
};

#endif
