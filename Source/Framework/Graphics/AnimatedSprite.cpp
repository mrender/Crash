#include "AnimatedSprite.h"
#include "../Events/UpdateEvent.h"
#include "../Services/Services.h"


namespace GameDev2D
{
    AnimatedSprite::AnimatedSprite(const std::string& aFilename) : SpriteAtlas(aFilename),
        m_FrameIndex(0),
        m_FrameSpeed(15.0f),
        m_ElapsedTime(0.0),
        m_DoesLoop(false),
		m_Pause(false)
    {
        //Add an event listener callback for the Update event
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

    AnimatedSprite::~AnimatedSprite()
    {
        Services::GetApplication()->RemoveEventListener(this, UPDATE_EVENT);
    }

    void AnimatedSprite::HandleEvent(Event* aEvent)
    {
        //Ensure the event is an Update event
        if (aEvent->GetEventCode() == UPDATE_EVENT)
        {
            //Get the detla time for the update event
            UpdateEvent* updateEvent = (UpdateEvent*)aEvent;
            double delta = updateEvent->GetDelta();

			//Check to see if the animation is paused or not
			if (m_Pause == false)
			{
				//Increment the elapsed time.
				m_ElapsedTime += delta;

				//Calculate the new frame index.
				int lastFrameIndex = m_FrameIndex;
				int frameIndex = (int)(m_ElapsedTime / (1.0 / m_FrameSpeed));

				//If the animation loops, we need to see if it is ready wrap around to the start.
				if (m_DoesLoop == true)
				{
					frameIndex = frameIndex % GetFrameCount();
				}

				//If the frame index has changed, set the new frame index.
				if (frameIndex != lastFrameIndex)
				{
					SetFrameIndex(frameIndex);
				}
			}
        }
    }

    void AnimatedSprite::AddFrame(const std::string& aAtlasKey)
    {
        //Add the frame
        m_Frames.push_back(aAtlasKey);

        //If that was the first frame, set the frame index
        if (m_Frames.size() == 1)
        {
            SetFrameIndex(0);
        }
    }

	void AnimatedSprite::RemoveAllFrames()
	{
		m_Frames.clear();
		SetFrameIndex(0);
	}

    unsigned long AnimatedSprite::GetFrameCount()
    {
        return m_Frames.size();
    }

    int AnimatedSprite::GetFrameIndex()
    {
        return m_FrameIndex;
    }

    void AnimatedSprite::SetFrameIndex(unsigned int aFrameIndex)
    {
        //Safety check the frame index.
        if (aFrameIndex < GetFrameCount())
        {
            //Set the frames index.
            m_FrameIndex = aFrameIndex;

            //Set the elapsed time
            m_ElapsedTime = (double)m_FrameIndex * (1.0 / m_FrameSpeed);

            //Set the SpriteAtlas's frame to the current frame using the atlas key
            UseFrame(m_Frames.at(m_FrameIndex));
        }
    }

    float AnimatedSprite::GetFrameSpeed()
    {
        return m_FrameSpeed;
    }

    void AnimatedSprite::SetFrameSpeed(float aFrameSpeed)
    {
        m_FrameSpeed = aFrameSpeed;
    }

    bool AnimatedSprite::DoesLoop()
    {
        return m_DoesLoop;
    }

    void AnimatedSprite::SetDoesLoop(bool aDoesLoop)
    {
        m_DoesLoop = aDoesLoop;
    }

	void AnimatedSprite::Pause()
	{
		m_Pause = true;
	}

	void AnimatedSprite::Resume()
	{
		m_Pause = false;
	}
}
