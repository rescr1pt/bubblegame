////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <ctime>
#include <Util.h>
#include <Game.h>

int main(int argc, char *argv[])
{
    const unsigned int now = (unsigned)time(nullptr);
    std::srand(now);
    sGame->Out(FS("\n\tSimpleBubble\n\tAuthor: rescr1pt@ya.ru \n\n"));

    sf::Vector2u winSize = GameDefault::WindowSize;

    if (argc == 3)
    {
        try
        {
            winSize.x = std::stoul(argv[1]);
            winSize.y = std::stoul(argv[2]);
        }
        catch (std::invalid_argument)
        {
            Assert(true, FS("Can't set wndow size! Error stratup argumnents '%%%x%%%'", argv[1], argv[2]));
        }
    }

    // Run game
    sGame->Run(winSize);

    return 0;
}

