#include "ResizeEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    ResizeEvent::ResizeEvent(Vector2 aSize) : Event(RESIZE_EVENT),
        m_Size(aSize)
    {
    }

    Vector2 ResizeEvent::GetSize()
    {
        return m_Size;
    }
    
    void ResizeEvent::LogEvent()
    {
        Log::Message(Log::Verbosity_Application, "[ResizeEvent] Size: (%f, %f)", m_Size.x, m_Size.y);
    }
}