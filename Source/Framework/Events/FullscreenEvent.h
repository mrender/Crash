#ifndef __GameDev2D__FullscreenEvent__
#define __GameDev2D__FullscreenEvent__

#include "Event.h"


namespace GameDev2D
{
    //The FullscreenEvent class is a subclass of the Event class and is sent by the
    //PlatformLayer whenever the application enteres or exits fullscreen mode
    class FullscreenEvent : public Event
    {
    public:
        FullscreenEvent(bool isFullscreen);
        
        //Retusn whether the application is in fullscreen mode or not
        bool IsFullscreen();
      
    protected:
        void LogEvent();
      
    private:
        bool m_IsFullscreen;
    };
}

#endif /* defined(__GameDev2D__FullscreenEvent__) */
