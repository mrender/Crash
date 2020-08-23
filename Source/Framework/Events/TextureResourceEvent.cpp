#include "TextureResourceEvent.h"
#include "../Debug/Log.h"
#include "../Graphics/Texture.h"


namespace GameDev2D
{
	TextureResourceEvent::TextureResourceEvent(Texture* aTexture, unsigned int aEventCode) : Event(aEventCode),
		m_Texture(aTexture)
	{

	}

	Texture* TextureResourceEvent::GetTexture()
	{
		return m_Texture;
	}

	void TextureResourceEvent::LogEvent()
	{
		Log::Message(Log::Verbosity_Resources, "[TextureResourceEvent] %s - %u", EventCodeToString(GetEventCode()).c_str(), m_Texture->GetId());
	}
}