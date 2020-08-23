#ifndef __GameDev2D__ResizeEvent__
#define __GameDev2D__ResizeEvent__

#include "Event.h"
#include "../Math/Vector2.h"


namespace GameDev2D
{
    //The ResizeEvent class is a subclass of the Event class and is sent by the
    //PlatformLayer whenever screen is resized
    class ResizeEvent : public Event
    {
    public:
        ResizeEvent(Vector2 size);
        
        //Returns the new size of the screen
        Vector2 GetSize();
      
    protected:
        void LogEvent();
      
    private:
        Vector2 m_Size;
    };
}

#endif
