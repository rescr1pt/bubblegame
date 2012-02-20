////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include "Util.h"

bool IsNumeric(std::string const& s)
{
    try
    {
        // Non-exception
        if (std::numeric_limits<float>::max() <= std::stof(s))
            return false;
    }
    catch (std::invalid_argument)
    {
        return false;
    }

    return true;
}

std::string CS(const char* format, ...)
{
    va_list ap;
    char str[1024];
    va_start(ap, format);
    vsnprintf(str, 1024, format, ap);
    va_end(ap);

    return str;
}

void Assert(bool check, std::string const& msg)
{
    if (!check)
        return;

    std::cerr << GetCurrentDateTime() << " " << msg;
    terminate();
}

// Get current date/time, format is YYYY-MM-DD.HH mm:ss
const std::string GetCurrentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

std::string const& StringTrim(std::string& source, std::string const& chrs)
{
    std::string::size_type pos = source.find_last_not_of(chrs);
    if(pos != std::string::npos) 
    {
        source.erase(pos + 1);
        pos = source.find_first_not_of(chrs);

        if(pos != std::string::npos) 
            source.erase(0, pos);
    }
    else 
        source.erase(source.begin(), source.end());

    return source;
}

Tokenize::Tokenize(std::string str, std::string const& delim, bool assertOnAccess, bool toLowerCase) : m_fullStr(str), m_assertOnAccess(assertOnAccess)
{
    std::string::size_type pos = 0, prevPos = 0;

    if (toLowerCase)
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    while ((pos = str.find(delim, pos)) != std::string::npos)
    {
        std::string tokened = str.substr(prevPos, pos - prevPos);
        
        m_tokens.push_back(StringTrim(tokened));
        prevPos = ++pos;          
    }

    // Last 
    std::string tokened = str.substr(prevPos, pos - prevPos);
    m_tokens.push_back(StringTrim(tokened));
}

std::string Tokenize::Get(uint32 elementIndex) const
{
	std::string element = "";
	
    try
    {
        element = m_tokens.at(elementIndex);
    }
    catch (std::out_of_range& e)
    {
        if (m_assertOnAccess)
            Assert(true, FS("Tokenize::Get: %%%! Can't tokenize '%%%'.", e.what(), m_fullStr));
    }
    
    return element; 
}
