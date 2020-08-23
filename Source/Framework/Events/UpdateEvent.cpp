#include "UpdateEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    UpdateEvent::UpdateEvent(double aDelta) : Event(UPDATE_EVENT),
        m_Delta(aDelta)
    {
    }

    double UpdateEvent::GetDelta()
    {
        return m_Delta;
    }

    void UpdateEvent::LogEvent()
    {
        //Log::Message(this, "UpdateEvent", Log::Verbosity_Application, "Delta: %f", m_Delta);
    }
}