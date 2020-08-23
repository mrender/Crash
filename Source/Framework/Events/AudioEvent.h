#pragma once

#include "Event.h"


namespace GameDev2D
{
	//Forward declaration
	class Audio;

	//The AudioEvent class is a subclass of the Event class and is sent by the
	//Audio class whenever audio playback starts or stops and when an audio loop ends
	class AudioEvent : public Event
	{
	public:
		AudioEvent(Audio* audio, unsigned int eventCode);

		//Returns a pointer to the Audio object that started or stopped
		Audio* GetAudio();

	protected:
		void LogEvent();

	private:
		Audio* m_Audio;
	};
}
