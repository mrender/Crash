#include "AudioEvent.h"
#include "../Audio/Audio.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
	AudioEvent::AudioEvent(Audio* aAudio, unsigned int aEventCode) : Event(aEventCode),
		m_Audio(aAudio)
	{

	}

	Audio* AudioEvent::GetAudio()
	{
		return m_Audio;
	}

	void AudioEvent::LogEvent()
	{
		Log::Message(Log::Verbosity_Audio, "[AudioEvent] %s", EventCodeToString(GetEventCode()).c_str());
	}
}