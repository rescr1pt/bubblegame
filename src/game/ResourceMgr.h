////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_RESOURCE_MGR_H
#define SIMPLEBUBBLE_RESOURCE_MGR_H

#include <unordered_map>

#include <Singleton.h>

// Forward decl 
namespace sf
{
    class Font;
    class Texture;
    class SoundBuffer;
}

enum class eResources : uint32
{
    None,

    // Font region
    Font_Region,
    Font_Dejavu,
    Font_Region_End,

    // Img region
    Img_Region,
    Img_Title,
    Img_Sound,
    Img_Region_End,

    // Audio region
    Audio_Region,
    Audio_Epiloge,
    Audio_Region_End,
};

/* TODO
template <class T>
class ResourceHolder
{
}; */

// Resource manager, need to refatoring (ResourceHolder).
class ResourceMgr
{
        friend class Singleton<ResourceMgr>;

    public:
        // <Pointer, Path>
        typedef std::pair<void*, std::string> ResourcePair;
        typedef std::unordered_map<eResources, ResourcePair> ResourceMap;

        void LoadResources();

        // Get resource (static_cast)
        template <class T> T* Get(eResources resourceType, bool assert);

        ResourceMap const& GetResources() const { return m_resources; }
        template <class T> void LoadResource(eResources resourceType, std::string const& path);
        ResourcePair* GetPair(eResources resourceType, bool assert) const;

        // Resource name (+file format)
        std::string GetName(eResources resourceType, bool assert) const;

    private:
        mutable ResourceMap m_resources;
};

// Simple accessors
#define GET_FONT(R) sResMgr->Get<sf::Font>(R, true)
#define GET_TEXTURE(R) sResMgr->Get<sf::Texture>(R, true)
#define GET_SOUND_BUFFER(R) sResMgr->Get<sf::SoundBuffer>(R, true)

#define sResMgr Singleton<ResourceMgr>::Inst()

#endif