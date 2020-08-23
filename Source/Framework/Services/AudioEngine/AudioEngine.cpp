#include "AudioEngine.h"
#include "../Services.h"
#include "../../Events/UpdateEvent.h"
#include <assert.h>
#include <xaudio2.h>
#pragma comment(lib, "Xaudio2")

namespace GameDev2D
{
	AudioEngine::AudioEngine() :
		m_Engine(nullptr),
		m_MasteringVoice(nullptr),
		m_Callback()
	{
		//Create the engine
		HRESULT result = XAudio2Create(&m_Engine);
		assert(SUCCEEDED(result));

		//Create the mastering voice
		result = m_Engine->CreateMasteringVoice(&m_MasteringVoice);
		assert(SUCCEEDED(result));

		//Add events to listen for suspend and resume events
		Services::GetApplication()->AddEventListener(this, SUSPEND_EVENT);
		Services::GetApplication()->AddEventListener(this, RESUME_EVENT);
	}

	AudioEngine::~AudioEngine()
	{
		Services::GetApplication()->RemoveEventListener(this, SUSPEND_EVENT);
		Services::GetApplication()->RemoveEventListener(this, RESUME_EVENT);
		m_MasteringVoice->DestroyVoice();
		m_Engine->Release();
	}

	void AudioEngine::HandleEvent(Event* aEvent)
	{
		if (aEvent != nullptr)
		{
			if (aEvent->GetEventCode() == SUSPEND_EVENT)
			{
				m_Engine->StopEngine();
			}
			else if (aEvent->GetEventCode() == RESUME_EVENT)
			{
				m_Engine->StartEngine();
			}
		}
	}

	void AudioEngine::CreateAudioVoice(IXAudio2SourceVoice** aSource, WAVEFORMATEX* aWaveFormat)
	{
		HRESULT result = m_Engine->CreateSourceVoice(aSource, aWaveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &m_Callback, nullptr, nullptr);
		assert(SUCCEEDED(result));
	}

	void AudioEngine::DestroyAudioVoice(IXAudio2SourceVoice* aSource)
	{
		if (aSource != nullptr)
		{
			aSource->DestroyVoice();
			aSource = nullptr;
		}
	}

	void AudioEngine::SetVolume(float aVolume)
	{
		m_MasteringVoice->SetVolume(aVolume);
	}

	float AudioEngine::GetVolume()
	{
		float volume = 0.0f;
		m_MasteringVoice->GetVolume(&volume);
		return volume;
	}
}