#pragma once

#include "../Events/EventDispatcher.h"
#include <xaudio2.h>
#include <string>


namespace GameDev2D
{
    //Audio class to handle playback of both music and sounds effects in game.
    class Audio : public EventDispatcher
    {
    public:
        //Constructor for the Audio class, many extensions are supported, use 'streamed' for longer files such as background music.
        //Don't stream for short files, such as sound effects. Audio files be looped.
        Audio(const std::string& filename);
        ~Audio();

        //Play and Stop methods
        void Play();
        void Stop();

        //Returns wether the audio is playing
        bool IsPlaying();

        //Sets the audio file loops or not
        void SetDoesLoop(bool doesLoop);

        //Returns the audio file loops or not
        bool DoesLoop();

		//Returns the number of channels 
		unsigned int GetNumberOfChannels();

		//Returns the sample rate
		unsigned int GetSampleRate();

        //Set the frequency of the audio file, must be positive
        void SetFrequencyRatio(float frequencyRatio);

        //Returns the current frequency of the audio file
        float GetFrequencyRatio();

        //Set the volume of the audio file, range 0.0f to 1.0f
        void SetVolume(float volume);

        //Returns the current volume of the audio file
        float GetVolume();

        //Sets the current position of the audio file, in samples
		void SetSample(unsigned long long sample);

		//Sets the current position of the audio file, in milleseconds
		void SetPositionMS(unsigned int milleseconds);

		//Sets the current position of the audio file, in seconds
		void SetPosition(double seconds);

        //Returns the current time of the audio file, in samples
        unsigned long long GetElapsedSamples();

		//Returns the current time of the audio file, in milleseconds
		unsigned int GetElapsedMS();

		//Returns the current time of the audio file, in seconds
		double GetElapsed();

        //Returns the remaining time of the audio file, in samples
        unsigned long long GetRemainingSamples();

		//Returns the remaining time of the audio file, in milleseconds
		unsigned int GetRemainingMS();

		//Returns the remaining time of the audio file, in seconds
		double GetRemaining();

		//Returns the number of samples of the audio object
		unsigned long long GetNumberOfSamples();

        //Returns the duration of the audio object, in milleseconds
		unsigned int GetDurationMS();

		//Returns the duration of the audio object, in seconds
		double GetDuration();

		//
		void DispatchEvent(Event& event);

    private:
        //Member variables
		IXAudio2SourceVoice* m_Source;
		WAVEFORMATEX m_WaveFormat;
		XAUDIO2_BUFFER m_Buffer;
		bool m_IsPlaying;
		unsigned long long m_SampleOffset;
    };
}
