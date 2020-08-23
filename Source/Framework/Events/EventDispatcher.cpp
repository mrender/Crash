#include "EventDispatcher.h"
#include "Event.h"
#include <assert.h>


namespace GameDev2D
{
	EventDispatcher::EventDispatcher() : EventHandler()
    {
    
	}
	
	EventDispatcher::~EventDispatcher()
    {
		
	}
    
    void EventDispatcher::RemoveAllHandlers()
    {
		m_HandlerEntries.clear();
	}
    
    void EventDispatcher::RemoveAllHandlersForListener(EventHandler* aHandler)
    {
		std::vector<std::pair<EventHandler*, unsigned int>>::iterator iter = m_HandlerEntries.begin();
		while (iter != m_HandlerEntries.end())
        {
			if((*iter).first == aHandler)
            {
				iter = m_HandlerEntries.erase(iter);
			}
            else
            {
				++iter;						
			}
		}
		
	}
    
    void EventDispatcher::AddEventListener(EventHandler* aHandler, unsigned int aEventCode)
    {
        //If you hit this assert, the event handler pointer you passed in was null
        assert(aHandler != nullptr);
        
        //Cycle through and check to make sure we haven't added the same handler for the event code already
        #if DEBUG
        for(unsigned int i = 0; i < m_HandlerEntries.size(); i++)
        {
            bool exists = m_HandlerEntries.at(i).first == aHandler && m_HandlerEntries.at(i).second == aEventCode;
            assert(exists == false);
        }
        #endif
        
        //Safety check the handler pointer, and add it to the handler entries
        if(aHandler != nullptr)
        {
            m_HandlerEntries.push_back(std::make_pair(aHandler, aEventCode));
        }
	}

	void EventDispatcher::RemoveEventListener(EventHandler* aHandler, unsigned int aEventCode)
    {
		for(unsigned int i = 0; i < m_HandlerEntries.size(); i++)
        {
			if(m_HandlerEntries.at(i).second == aEventCode && m_HandlerEntries.at(i).first == aHandler)
            {
				m_HandlerEntries.erase(m_HandlerEntries.begin()+i);
			}
		}
	}
	
    void EventDispatcher::DispatchEvent(Event& aEvent)
    {
        //Set the event's dispatcher and event code
        aEvent.SetDispatcher(this);

        //Log the event that is about to be dispatched
        aEvent.LogEvent();

        //Cycle through the event entries and handle the event's that match the event code
        for (unsigned int i = 0; i < m_HandlerEntries.size(); i++)
        {
            if (m_HandlerEntries.at(i).second == aEvent.GetEventCode())
            {
                //Lastly call the event handler to handle the event
                m_HandlerEntries.at(i).first->HandleEvent(&aEvent);
            }
        }
    }
}