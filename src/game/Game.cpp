////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <Game.h>
#include <Util.h>
#include <TitleScene.h>
#include <ResourceMgr.h>

// Battlefield cell defaults
const std::string GameDefault::WindowTitle = "SimpleBubble";
const sf::Vector2u GameDefault::WindowSize = sf::Vector2u(800, 600);

Game::Game() : m_scene(nullptr), m_mouseEnabled(false), m_mousePressed(false), m_renderWindow(nullptr), m_sound(nullptr)
{
}

Game::~Game()
{
    if (m_sound)
        m_sound->stop();

    delete m_sound;
    delete m_scene;
    delete m_renderWindow;
}

void Game::Run(sf::Vector2u const& windowSize)
{
    sGame->Out(FS("Run game with size %%%x%%% ... ", windowSize.x, windowSize.y));

    Assert((m_renderWindow != nullptr), "Game::Run: Game is already running!");

    // Create window
    m_renderWindow = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), GameDefault::WindowTitle, sf::Style::Close);
    m_renderWindow->setFramerateLimit(60);

    sResMgr->LoadResources();

    // Set bg sound
    m_sound = new sf::Sound;
    m_sound->setBuffer(*GET_SOUND_BUFFER(eResources::Audio_Epiloge));
    m_sound->setLoop(true);
    m_sound->setVolume(50);
    // Diversity
    m_sound->setPitch(RandF<float>(0.9f, 1.1f));
    m_sound->play();

    m_scene = new TitleScene;

    // Background color sf::Color::Black
    sf::Color const& bgColor = sf::Color(0, 0, 0);

    sGame->Out("Game is runned!");

    // Game loop
    while (m_renderWindow->isOpen())
    {
        m_mousePressed = false;

		// SFML events
        while (m_renderWindow->pollEvent(m_event))
        {
			// Event types 
            switch (GetEvent().type)
            {
                case sf::Event::Closed:
                    m_renderWindow->close();
                    break;
                case sf::Event::MouseButtonPressed:
                    m_mouseEnabled = !m_mouseEnabled;
                    m_mousePressed = true;
                    break;
            }
        }

        // Update Scene
        GetScene()->Update(m_mainClock);

        // Draw scene
        m_renderWindow->clear(bgColor);
        Assert(!m_scene, "No specified scene for rendering!");
        m_renderWindow->draw(*m_scene);
        m_renderWindow->display();

        // Handbrake
        sf::sleep(sf::microseconds(60));
    }

    sGame->Out("Game is exited!");
}

void Game::Out(std::string const& text)
{
    std::string outMsg = GetCurrentDateTime() + " ";
    outMsg.append(text);

    std::cout << outMsg << std::endl;
}

void Game::SwitchToScene(Scene* scene)
{
    delete m_scene;
    m_scene = nullptr;
    m_scene = scene;
}
