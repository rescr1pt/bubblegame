////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_UTIL_H
#define SIMPLEBUBBLE_UTIL_H

#include <random>

// Random generator
template <class T> T RandI(T t1, T t2);
template <class T> T RandF(T t1, T t2);
template <class T> T Roll(T chance);

// C-style format string
std::string CS(const char* format, ...);
// C++11-style format string
template <class ... T> 
inline std::string const& FS(std::string const& format);
template <class Head, class ... Tail> 
inline std::string FS(std::string const& format, Head const& head, Tail const& ... tail);

// Will approach, for small numbers
bool IsNumeric(std::string const& s);
void Assert(bool check, std::string const& msg);
const std::string GetCurrentDateTime();
std::string const& StringTrim(std::string& source, std::string const& chrs = " ");

class Tokenize
{
    public:
        typedef std::vector<std::string> TokenMap;

        explicit Tokenize(std::string str, std::string const& delim, bool assertOnAccess, bool toLowerCase = false);

        std::string Get(uint32 elementIndex) const;
        TokenMap const& GetTokens() const { return m_tokens; }

    private:
        std::string m_fullStr;
        TokenMap m_tokens;
        bool m_assertOnAccess;
};

#include <Util.inl>

#endif