////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <Game.h>
#include <ResourceMgr.h>
#include <Util.h>

void ResourceMgr::LoadResources()
{
    for (std::pair<const eResources, ResourcePair> & p : m_resources)
    {
        delete p.second.first;
        p.second.second.clear();
    }

    m_resources.clear();

    // Font
    LoadResource<sf::Font>(eResources::Font_Dejavu, "res/font/dejavusans.ttf");

    // Img
    LoadResource<sf::Texture>(eResources::Img_Title, "res/img/title.png");
    LoadResource<sf::Texture>(eResources::Img_Sound, "res/img/sound.png");

    // Audio
    LoadResource<sf::SoundBuffer>(eResources::Audio_Epiloge, "res/audio/epiloge.ogg");
}

template <class T> void ResourceMgr::LoadResource(eResources resourceType, std::string const& path)
{
    T* TResource = new T();

    if (!TResource->loadFromFile(path))
        Assert(true, FS("ResourceMgr<T>::LoadResource: Can't load resource from '%%%'.", path));

    m_resources[resourceType] = std::make_pair(TResource, path);

    sGame->Out(FS("Load '%%%'", path));
}

template <class T> T* ResourceMgr::Get(eResources resourceType, bool assert)
{
    ResourcePair* pair = GetPair(resourceType, assert);

    if (!pair)
        return nullptr;

    T* resPtr = static_cast<T*>(pair->first);

    if (assert && !resPtr)
        Assert(true, FS("ResourceMgr<T>::GetResource: Unknown '%%%' pointer!", (uint16)resourceType));

    return resPtr;
}

ResourceMgr::ResourcePair* ResourceMgr::GetPair(eResources resourceType, bool assert) const
{
    const ResourceMap::iterator it = m_resources.find(resourceType);

    if (it == m_resources.end())
    {
        if (assert)
            Assert(true, FS("ResourceMgr::GetPair: Can't found resource pair for type '%%%'!", (uint16)resourceType));
        else
            return nullptr;
    }

    return &(it->second);
}

std::string ResourceMgr::GetName(eResources resourceType, bool assert) const
{
    const ResourcePair* pair = GetPair(resourceType, assert);

    // If the pair not found and not asserted, then it will return an empty string
    if (!pair)
        return "";

    std::string const& resourcePath = pair->second;
    const char dir = '/';

    std::string::size_type nameN = resourcePath.rfind(dir);

    // Not found name position 
    if (nameN == std::string::npos)
    {
        if (assert)
            Assert(true, FS("ResourceMgr::GetName: Not found resource name for '%%%'!", resourcePath));
        else
            return "";
    }

    // Skip 'dir'
    std::string& name = resourcePath.substr(nameN + 1);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    return name;
}

// Specialize
template void ResourceMgr::LoadResource<sf::Font>(eResources, std::string const&);
template void ResourceMgr::LoadResource<sf::Texture>(eResources, std::string const&);
template void ResourceMgr::LoadResource<sf::SoundBuffer>(eResources, std::string const&);
template sf::Font* ResourceMgr::Get<sf::Font>(eResources, bool);
template sf::Texture* ResourceMgr::Get<sf::Texture>(eResources, bool);
template sf::SoundBuffer* ResourceMgr::Get<sf::SoundBuffer>(eResources, bool);