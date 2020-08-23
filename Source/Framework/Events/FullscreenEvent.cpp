#include "FullscreenEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    FullscreenEvent::FullscreenEvent(bool aIsFullscreen) : Event(FULLSCREEN_EVENT),
        m_IsFullscreen(aIsFullscreen)
    {

    }
    
    bool FullscreenEvent::IsFullscreen()
    {
        return m_IsFullscreen;
    }
    
    void FullscreenEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Application, "[FullscreenEvent] Fullscreen: %s", m_IsFullscreen == true ? "true" : "false");
    }
}