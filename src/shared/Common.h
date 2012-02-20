////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_COMMON_H
#define SIMPLEBUBBLE_COMMON_H

#include <stddef.h>
#include <string>
#include <limits>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <Define.h>

enum eTimeConstants
{
    MINUTE = 60,
    HOUR = MINUTE * 60,
    DAY = HOUR * 24,
    WEEK = DAY * 7,
    MONTH = DAY * 30,
    YEAR = MONTH * 12,
    IN_MILLISECONDS = 1000,
    IN_MICROSECONDS = IN_MILLISECONDS * IN_MILLISECONDS,
};

#define nullvec(T) sf::Vector2<T>(0, 0)
#define nullvec2f  sf::Vector2f(0.f, 0.f)

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#endif