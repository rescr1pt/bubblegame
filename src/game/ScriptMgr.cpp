////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <unordered_map>

#include <SFML/System/Mutex.hpp>

#include <ScriptMgr.h>
#include <Util.h>
#include <Bubble.h>

template<class TScript>
class ScriptRegistry : public sf::NonCopyable
{
    public:
        typedef std::unordered_map<std::string, TScript*> ScriptMap;
        static sf::Mutex m_mutex;
        static ScriptMap m_scripts;

        static void Add(TScript* script)
        {
            if (m_scripts.find(script->GetScriptName()) != m_scripts.end())
                Assert(true, FS("ScriptRegistry<T>::Add Script '%%%' is already in holder!", script->GetScriptName()));

            // Check pointer has?

            m_mutex.lock();
            m_scripts[script->GetScriptName()] = script;
            m_mutex.unlock();
        }

        static void Remove(TScript* script)
        {
            m_mutex.lock();
            m_scripts.erase(script->GetScriptName());
            m_mutex.unlock();
        }

        static TScript* Get(std::string const& scriptName)
        {
            ScriptMap::iterator it = m_scripts.find(scriptName);
            return it != m_scripts.end() ? it->second : nullptr;
        }
};

// Range-based 
#define FOREACH_SCRIPT(T) \
    for (std::pair<const std::string, T*> & script : ScriptRegistry<T>::m_scripts) \
        script.second

// BubbleScript
BubbleScript::BubbleScript(std::string const& scriptName) : ScriptObject(scriptName)
{
    ScriptRegistry<BubbleScript>::Add(this);
}

BubbleScript::~BubbleScript()
{
    ScriptRegistry<BubbleScript>::Remove(this);
}

void ScriptMgr::OnBubbleCreate(Bubble* bubble)
{
    FOREACH_SCRIPT(BubbleScript)->OnBubbleCreate(bubble);
}

void ScriptMgr::OnBubbleDestroy(Bubble* bubble)
{
    FOREACH_SCRIPT(BubbleScript)->OnBubbleDestroy(bubble);
}

void ScriptMgr::OnBubbleMove(Bubble* bubble)
{
    FOREACH_SCRIPT(BubbleScript)->OnBubbleMove(bubble);
}

// Initial static members
template <class TScript> std::unordered_map<std::string, TScript*> ScriptRegistry<TScript>::m_scripts;
template <class TScript> sf::Mutex ScriptRegistry<TScript>::m_mutex;

// Specialize for each script
template class ScriptRegistry<BubbleScript>;
