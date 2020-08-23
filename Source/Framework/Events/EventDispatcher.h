#ifndef __GameDev2D__EventDispatcher__
#define __GameDev2D__EventDispatcher__

#include "EventHandler.h"
#include <vector>


namespace GameDev2D
{
    //Forward declaration
	class Event;

    //The EventDispatcher can be inherited from to handle dispatching of events to listeners. It handles
    //the adding and removing of listener for the inheriting class. Simple call the DispatchEvent()
    //method and it will send the Event to any EventHandlers that are listening for the event.
	class EventDispatcher : public EventHandler
    {
    public:
        EventDispatcher();
        virtual ~EventDispatcher();
		
        //Dispatches an Event, by default the event will be deleted after it is dispatch, however if you
        //pass in false as the 2nd argument then the event won't be deleted. This method can be overridden.
        virtual void DispatchEvent(Event& event);
        
        //Add an event handler for a specific event code, an event will be dispatched to the handler
        //when the event is triggered.
        void AddEventListener(EventHandler* handler, unsigned int eventCode);
        
        //Removes a specific event code for a handler from the dispatcher.
        void RemoveEventListener(EventHandler* handler, unsigned int eventCode);
			
        //Removes handler and all event codes associated with it  from the dispatcher.
        void RemoveAllHandlersForListener(EventHandler* handler);
        
        //Removes all the handlers from the dispatcher.
        void RemoveAllHandlers();
        
    protected:
        //Member variable
		std::vector<std::pair<EventHandler*, unsigned int>> m_HandlerEntries;
	};
}

#endif
