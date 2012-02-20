////////////////////////////////////////////////////////////
//
// SimpleBubble
// Contact: rescr1pt@ya.ru
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEBUBBLE_SINGLETON_H
#define SIMPLEBUBBLE_SINGLETON_H

#include <SFML/System/NonCopyable.hpp>

// Simple Singleton system
template <class T> class Singleton : protected sf::NonCopyable
{
    public:
        static T* Inst();
        void FreeInst();

    protected:

        virtual ~Singleton()
        {
            m_self = nullptr;
        }

    private:
        static T* m_self;
        static int m_refCount;
};

template <class T> T* Singleton<T>::m_self = nullptr;

template <class T> int Singleton<T>::m_refCount = 0;

template <class T> T* Singleton<T>::Inst()
{
    if (!m_self)
        m_self = new T;

    ++m_refCount;
    return m_self;
}

template <class T> void Singleton<T>::FreeInst()
{
    if (--m_refCount == 0)
        delete this;
}

#endif