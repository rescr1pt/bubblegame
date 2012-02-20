////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_SCRIPTMGR
#define SIMPLEBUBBLE_SCRIPTMGR

#include <Singleton.h>

class Bubble;
class ScriptMgr;

class ScriptObject
{
        friend class ScriptMgr;
    public:
        std::string const& GetScriptName() const { return m_scriptName; }

    protected:
        virtual ~ScriptObject() {}
        ScriptObject(std::string const& scriptName) : m_scriptName(scriptName) {}

    protected:
        std::string m_scriptName;
};

class BubbleScript : public ScriptObject
{
    protected:
        BubbleScript(std::string const& scriptName);
        virtual ~BubbleScript();

    public:
        virtual void OnBubbleCreate(Bubble* bubble) {}
        virtual void OnBubbleDestroy(Bubble* bubble) {}
        virtual void OnBubbleMove(Bubble* bubble) {}
};

class ScriptMgr
{
        friend Singleton<ScriptMgr>;
	
    public:

        // BubbleScript
        void OnBubbleCreate(Bubble* bubble);
        void OnBubbleDestroy(Bubble* bubble);
        void OnBubbleMove(Bubble* bubble);
};

#define sScriptMgr Singleton<ScriptMgr>::Inst()

#endif