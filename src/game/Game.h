////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_GAME_H
#define SIMPLEBUBBLE_GAME_H

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include <Singleton.h>

struct GameDefault
{
    static const std::string WindowTitle;
    static const sf::Vector2u WindowSize;
};

// Forward decl
class Scene;

namespace sf
{
    class RenderWindow;
    class Sound;
};

class Game 
{
        friend class Singleton<Game>;

    private:
        Game();
        
    public:
        ~Game();

        void Run(sf::Vector2u const& windowSize = GameDefault::WindowSize);
        void Out(std::string const& text);

        // Scene
        void SwitchToScene(Scene* scene);
        Scene* GetScene() const { return m_scene; }

        // SFML 
        sf::RenderWindow* GetWindow() const { return m_renderWindow; }
        sf::Event const& GetEvent() const { return m_event; }
        sf::Clock const& GetClock() const { return m_mainClock; }
        sf::Sound* GetSound() const { return m_sound; }

        bool MouseEnabled() const { return m_mouseEnabled; }
        bool MousePressed() const { return m_mousePressed; }

    private: 
        Scene* m_scene;
        bool m_mouseEnabled;
        bool m_mousePressed;

        // SFML
        sf::RenderWindow* m_renderWindow;
        sf::Event m_event;
        sf::Clock m_mainClock;
        sf::Sound* m_sound;
};

#define sGame Singleton<Game>::Inst()

#endif