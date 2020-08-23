#pragma once

#include "Audio.h"
#include "../Events/AudioEvent.h"
#include <xaudio2.h>


namespace GameDev2D
{
	struct WaveData
	{
		WaveData()
		{
			ZeroMemory(&waveFormat, sizeof(waveFormat));
			ZeroMemory(&buffer, sizeof(buffer));
			data = nullptr;
		}

		~WaveData()
		{
			delete[] data;
		}

		WAVEFORMATEX waveFormat;
		XAUDIO2_BUFFER buffer;
		void* data;
	};

	struct AudioCallback : public IXAudio2VoiceCallback
	{
		AudioCallback()
		{
		}

		virtual ~AudioCallback()
		{
		}

		__declspec(nothrow) void __stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
		__declspec(nothrow) void __stdcall OnVoiceProcessingPassEnd() {}
		__declspec(nothrow) void __stdcall OnStreamEnd() {}
		__declspec(nothrow) void __stdcall OnBufferStart(void* context)
		{
			if (context != nullptr)
			{
				Audio* audio = reinterpret_cast<Audio*>(context);
				audio->DispatchEvent(AudioEvent(audio, AUDIO_PLAYBACK_STARTED));
			}
		}

		__declspec(nothrow) void __stdcall OnBufferEnd(void* context)
		{
			if (context != nullptr)
			{
				Audio* audio = reinterpret_cast<Audio*>(context);
				audio->DispatchEvent(AudioEvent(audio, AUDIO_PLAYBACK_ENDED));
			}
		}

		__declspec(nothrow) void __stdcall OnLoopEnd(void* context) 
		{
			if (context != nullptr)
			{
				Audio* audio = reinterpret_cast<Audio*>(context);
				audio->DispatchEvent(AudioEvent(audio, AUDIO_LOOP_ENDED));
			}
		}
		__declspec(nothrow) void __stdcall OnVoiceError(void* context, HRESULT) {}
	};
}