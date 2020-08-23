#ifndef __GameDev2D__Event__
#define __GameDev2D__Event__

#include <string>


namespace GameDev2D
{
    //Forward declaration
    class EventDispatcher;
    
    //Platform events
    const unsigned int UNKNOWN_EVENT = 0;
    const unsigned int SHUTDOWN_EVENT = 1;
    const unsigned int SUSPEND_EVENT = 2;
    const unsigned int RESUME_EVENT = 3;
    const unsigned int UPDATE_EVENT = 4;
    const unsigned int DRAW_EVENT = 5;
    const unsigned int RESIZE_EVENT = 6;
    const unsigned int FULLSCREEN_EVENT = 7;

	//Resource events
	const unsigned int TEXTURE_RESOURCE_LOADED = 20;
	const unsigned int TEXTURE_RESOURCE_UNLOADED = 21;

	//Audio events
	const unsigned int AUDIO_PLAYBACK_STARTED = 22;
	const unsigned int AUDIO_PLAYBACK_ENDED = 23;
	const unsigned int AUDIO_LOOP_ENDED = 24;

    //Key events
    const unsigned int KEY_UP_EVENT = 25;
    const unsigned int KEY_DOWN_EVENT = 26;
    const unsigned int KEY_REPEAT_EVENT = 27;
    
    //Mouse events
    const unsigned int MOUSE_MOVEMENT_EVENT = 28;
    const unsigned int MOUSE_BUTTON_DOWN_EVENT = 29;
    const unsigned int MOUSE_BUTTON_UP_EVENT = 30;
    const unsigned int MOUSE_WHEEL_EVENT = 31;
    
    //GamePad events
    const unsigned int GAMEPAD_CONNECTED_EVENT = 32;
    const unsigned int GAMEPAD_DISCONNECTED_EVENT = 33;
    const unsigned int GAMEPAD_BUTTON_UP_EVENT = 34;
    const unsigned int GAMEPAD_BUTTON_DOWN_EVENT = 35;
    const unsigned int GAMEPAD_LEFT_TRIGGER_EVENT = 36;
    const unsigned int GAMEPAD_RIGHT_TRIGGER_EVENT = 37;
    const unsigned int GAMEPAD_LEFT_THUMBSTICK_EVENT = 38;
    const unsigned int GAMEPAD_RIGHT_THUMBSTICK_EVENT = 39;


    //The Event class, can be used as is to send events (like platform events) or can be
    //overridden to add additonal functionality (like the input events). It is the main
    //interface for events that are sent by the EventDispatcher.
    class Event
    {
    public:
        //The constructor takes in the eventCode, as well as optional eventData,
        //which can be any type of pointer.
        Event(int eventCode);
        virtual ~Event();
        
        //Returns the event code of the event
        unsigned int GetEventCode();
        
        //Returns the EventDispatcher
        EventDispatcher* GetDispatcher() const;
        
        //Converts an event code to a string
        static std::string EventCodeToString(unsigned int eventCode);

    protected:
        //Can be called by inheriting classes to set the event code. Can be overridden.
        virtual void SetEventCode(unsigned int eventCode);

        //Called right before the event is dispatched, it logs an description of the event that is triggered, Can be overridden.
        virtual void LogEvent();
        
        //Called by the EventDispatcher to set the dispatcher
        void SetDispatcher(EventDispatcher* dispatcher);
    
        //The EventDispatcher is a friend of Event and calls the SetDispatcher() method
        friend class EventDispatcher;
    
    private:
        //Member variables
        unsigned int m_EventCode;
        EventDispatcher* m_Dispatcher;
    };
}

#endif
