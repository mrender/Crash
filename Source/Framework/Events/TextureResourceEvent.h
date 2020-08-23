#pragma once

#include "Event.h"


namespace GameDev2D
{
	//Forward declaration
	class Texture;

	//The AudioEvent class is a subclass of the Event class and is sent by the
	//Audio class whenever audio playback starts or stops and when an audio loop ends
	class TextureResourceEvent : public Event
	{
	public:
		TextureResourceEvent(Texture* texture, unsigned int eventCode);

		//Returns a pointer to the Audio object that started or stopped
		Texture* GetTexture();

	protected:
		void LogEvent();

	private:
		Texture* m_Texture;
	};
}
