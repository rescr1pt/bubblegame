////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <sstream>

// Format string specifier. Macro is used to catch initialized.
#define FORMAT_SPECIFIER std::string("%%%")

// Random generator (global)
static std::random_device g_randomDevice;
static std::mt19937 g_mt199937(g_randomDevice());

template <class T> T RandI(T t1, T t2)
{
	std::uniform_int_distribution<T> dis(t1, t2);
	return dis(g_mt199937);
}

template <class T> T RandF(T t1, T t2)
{
    std::uniform_real_distribution<T> dis(t1, t2);
	return dis(g_mt199937);
}

template <class T> T Roll(T chance)
{
    return chance > Rand<T>(0, 99);
}

template <class ... T>
std::string const& FS(std::string const& format)
{
    const size_t firstPlaceholderPos = format.find(FORMAT_SPECIFIER);

    if (firstPlaceholderPos != std::string::npos)
        Assert(true, CS("FS: Error formating '%s'! First place holder = '%u'.", format.c_str(), firstPlaceholderPos));

    return format;
}

template <class Head, class ... Tail>
std::string FS(std::string const& format, Head const& head, Tail const& ... tail)
{
    std::stringstream os;

    size_t firstPlaceholderPos = format.find(FORMAT_SPECIFIER);

    if (firstPlaceholderPos == std::string::npos)
        Assert(true, CS("FS: Error formating '%s'! Can't found first place position!", format.c_str()));
    else
    {
        std::string front(format, 0, firstPlaceholderPos);
        std::string back(format, firstPlaceholderPos + FORMAT_SPECIFIER.size());

        os << front << head << FS(back, tail ... );
    }

    return os.str();
}