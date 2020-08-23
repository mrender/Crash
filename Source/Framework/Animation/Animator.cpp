#include "Animator.h"
#include "../Events/UpdateEvent.h"
#include "../Math/Math.h"
#include "../Services/Services.h"
#include "../Windows/Application.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    ///////////////////////////////////////////////////////////////////////////
    // Animator
    //
    Animator::Animator() : EventHandler(),
        m_Time(0.0),
        m_IsAnimating(false)
    {

    }

    Animator::~Animator()
    {

    }

    void Animator::HandleEvent(Event* aEvent)
    {
        if (aEvent != nullptr)
        {
            if (aEvent->GetEventCode() == UPDATE_EVENT)
            {
                //Cast the update event
                UpdateEvent* updateEvent = static_cast<UpdateEvent*>(aEvent);

                //Update the animator logic
                Update(updateEvent->GetDelta());
            }
        }
    }

    void Animator::Start()
    {
        //Are we already animating?
        if (IsAnimating() == false)
        {
            //Set the animation flag to true
            m_IsAnimating = true;

            //Prepare the first frame for animation
            NextKeyFrame();
        }
    }

    void Animator::Stop()
    {
        m_IsAnimating = false;
    }

    bool Animator::IsAnimating()
    {
        return m_IsAnimating;
    }


    ///////////////////////////////////////////////////////////////////////////
    // AnimatorFloat
    //
    AnimatorFloat::AnimatorFloat(SetterFloatCallback aSetterCallback, GetterFloatCallback aGetterCallback) : Animator(),
        m_SetterCallback(aSetterCallback),
        m_GetterCallback(aGetterCallback),
        m_Reference(nullptr),
        m_Start(0.0f),
        m_End(0.0f)
    {
        //Add an event listener to listen for Update events
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

    AnimatorFloat::AnimatorFloat(float* aReference) : Animator(),
        m_SetterCallback(nullptr),
        m_GetterCallback(nullptr),
        m_Reference(aReference),
        m_Start(0.0f),
        m_End(0.0f)
    {
        //Add an event listener to listen for Update events
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

    AnimatorFloat::~AnimatorFloat()
    {
        //Make sure to remove the event listener
        Services::GetApplication()->RemoveEventListener(this, UPDATE_EVENT);
    }

    void AnimatorFloat::AddKeyframe(float aValue, double aDuration, EasingFunction aEasingFunction)
    {
        m_Keyframes.push_back(Keyframe(aValue, aDuration, aEasingFunction));
    }

    void AnimatorFloat::Update(double aDelta)
    {
        //Are we animating?
        if (IsAnimating() == true)
        {
            //Get the first keyframe
            Keyframe keyframe = m_Keyframes.front();

            //Increment the animation time
            m_Time += aDelta;

            //If the time has exceeded the duration, cap it
            if (m_Time >= keyframe.duration)
            {
                m_Time = keyframe.duration;
            }

            //Calculate the percentage of the time that is completed
            float percentage = static_cast<float>(m_Time / keyframe.duration);
            float current = 0.0f;

            //Calculate the current value (wether an easing function is present or not
            if (keyframe.easingFunction != nullptr)
            {
                current = m_Start + (m_End - m_Start) * keyframe.easingFunction(percentage);
            }
            else
            {
                current = m_Start + (m_End - m_Start) * percentage;
            }

            //Set the object's current value using the callback method
            m_Reference == nullptr ? m_SetterCallback(current) : *m_Reference = current;

            //Has the time reach its duration
            if (m_Time == keyframe.duration)
            {
                //Remove the first keyframe in the list
                m_Keyframes.pop_front();

                //If there are no more keyframes left, then we are done animating
                if (m_Keyframes.size() == 0)
                {
                    m_IsAnimating = false;
                }
                else
                {
                    NextKeyFrame();
                }
            }
        }
    }

    void AnimatorFloat::NextKeyFrame()
    {
        //Ensure there are keyframes to animate
        if (m_Keyframes.size() > 0)
        {
            //Initialize the animation values for a new frame
            m_Start = (m_Reference == nullptr ? m_GetterCallback() : *m_Reference);
            m_End = m_Keyframes.front().value;
            m_Time = 0.0;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // AnimatorVector2
    //
    AnimatorVector2::AnimatorVector2(SetterVector2Callback aSetterCallback, GetterVector2Callback aGetterCallback) : Animator(),
        m_SetterCallback(aSetterCallback),
        m_GetterCallback(aGetterCallback),
        m_Reference(nullptr),
        m_Start(Vector2(0.0f, 0.0f)),
        m_End(Vector2(0.0f, 0.0f))
    {
        //Add an event listener to listen for Update events
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

    AnimatorVector2::AnimatorVector2(Vector2* aReference) : Animator(),
        m_SetterCallback(nullptr),
        m_GetterCallback(nullptr),
        m_Reference(aReference),
        m_Start(Vector2(0.0f, 0.0f)),
        m_End(Vector2(0.0f, 0.0f))
    {
        //Add an event listener to listen for Update events
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

    AnimatorVector2::~AnimatorVector2()
    {
        //Make sure to remove the event listener
        Services::GetApplication()->RemoveEventListener(this, UPDATE_EVENT);
    }

    void AnimatorVector2::AddKeyframe(Vector2 aValue, double aDuration, EasingFunction aEasingFunction)
    {
        m_Keyframes.push_back(Keyframe(aValue, aDuration, aEasingFunction));
    }

    void AnimatorVector2::Update(double aDelta)
    {
        //Are we animating?
        if (IsAnimating() == true)
        {
            //Get the first keyframe
            Keyframe keyframe = m_Keyframes.front();

            //Increment the animation time
            m_Time += aDelta;

            //If the time has exceeded the duration, cap it
            if (m_Time >= keyframe.duration)
            {
                m_Time = keyframe.duration;
            }

            //Calculate the percentage of the time that is completed
            float percentage = static_cast<float>(m_Time / keyframe.duration);
            Vector2 current = Vector2(0.0f, 0.0f);

            //Calculate the current value (wether an easing function is present or not
            if (keyframe.easingFunction != nullptr)
            {
                current = m_Start + (m_End - m_Start) * keyframe.easingFunction(percentage);
            }
            else
            {
                current = m_Start + (m_End - m_Start) * percentage;
            }

            //Set the object's current value using the callback method
            m_Reference == nullptr ? m_SetterCallback(current) : *m_Reference = current;

            //Has the time reach its duration
            if (m_Time == keyframe.duration)
            {
                //Remove the first keyframe in the list
                m_Keyframes.pop_front();

                //If there are no more keyframes left, then we are done animating
                if (m_Keyframes.size() == 0)
                {
                    m_IsAnimating = false;
                }
                else
                {
                    NextKeyFrame();
                }
            }
        }
    }

    void AnimatorVector2::NextKeyFrame()
    {
        //Ensure there are keyframes to animate
        if (m_Keyframes.size() > 0)
        {
            //Initialize the animation values for a new frame
            m_Start = (m_Reference == nullptr ? m_GetterCallback() : *m_Reference);
            m_End = m_Keyframes.front().value;
            m_Time = 0.0;
        }
    }



    ///////////////////////////////////////////////////////////////////////////
    // AnimatorColor
    //
    AnimatorColor::AnimatorColor(SetterColorCallback aSetterCallback, GetterColorCallback aGetterCallback) : Animator(),
        m_SetterCallback(aSetterCallback),
        m_GetterCallback(aGetterCallback),
        m_Reference(nullptr),
        m_Start(Color::WhiteColor()),
        m_End(Color::WhiteColor())
    {
        //Add an event listener to listen for Update events
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

    AnimatorColor::AnimatorColor(Color* aReference) : Animator(),
        m_SetterCallback(nullptr),
        m_GetterCallback(nullptr),
        m_Reference(aReference),
        m_Start(Color::WhiteColor()),
        m_End(Color::WhiteColor())
    {
        //Add an event listener to listen for Update events
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

    AnimatorColor::~AnimatorColor()
    {
        //Make sure to remove the event listener
        Services::GetApplication()->RemoveEventListener(this, UPDATE_EVENT);
    }

    void AnimatorColor::AddKeyframe(Color aValue, double aDuration, EasingFunction aEasingFunction)
    {
        m_Keyframes.push_back(Keyframe(aValue, aDuration, aEasingFunction));
    }

    void AnimatorColor::Update(double aDelta)
    {
        //Are we animating?
        if (IsAnimating() == true)
        {
            //Get the first keyframe
            Keyframe keyframe = m_Keyframes.front();

            //Increment the animation time
            m_Time += aDelta;

            //If the time has exceeded the duration, cap it
            if (m_Time >= keyframe.duration)
            {
                m_Time = keyframe.duration;
            }

            //Calculate the percentage of the time that is completed
            float percentage = static_cast<float>(m_Time / keyframe.duration);
            Color current = Color::WhiteColor();

            //Calculate the current value (wether an easing function is present or not
            if (keyframe.easingFunction != nullptr)
            {
                current.r = m_Start.r + (m_End.r - m_Start.r) * keyframe.easingFunction(percentage);
                current.g = m_Start.g + (m_End.g - m_Start.g) * keyframe.easingFunction(percentage);
                current.b = m_Start.b + (m_End.b - m_Start.b) * keyframe.easingFunction(percentage);
                current.a = m_Start.a + (m_End.a - m_Start.a) * keyframe.easingFunction(percentage);
            }
            else
            {
                current.r = m_Start.r + (m_End.r - m_Start.r) * percentage;
                current.g = m_Start.g + (m_End.g - m_Start.g) * percentage;
                current.b = m_Start.b + (m_End.b - m_Start.b) * percentage;
                current.a = m_Start.a + (m_End.a - m_Start.a) * percentage;
            }

            //Clamp the color values to make sure they stay in range (0.0 - 1.0)
            current.r = Math::Clamp(current.r, 0.0f, 1.0f);
            current.g = Math::Clamp(current.g, 0.0f, 1.0f);
            current.b = Math::Clamp(current.b, 0.0f, 1.0f);
            current.a = Math::Clamp(current.a, 0.0f, 1.0f);

            //Set the object's current value using the callback method
            m_Reference == nullptr ? m_SetterCallback(current) : *m_Reference = current;

            //Has the time reach its duration
            if (m_Time == keyframe.duration)
            {
                //Remove the first keyframe in the list
                m_Keyframes.pop_front();

                //If there are no more keyframes left, then we are done animating
                if (m_Keyframes.size() == 0)
                {
                    m_IsAnimating = false;
                }
                else
                {
                    NextKeyFrame();
                }
            }
        }
    }

    void AnimatorColor::NextKeyFrame()
    {
        //Ensure there are keyframes to animate
        if (m_Keyframes.size() > 0)
        {
            //Initialize the animation values for a new frame
            m_Start = (m_Reference == nullptr ? m_GetterCallback() : *m_Reference);
            m_End = m_Keyframes.front().value;
            m_Time = 0.0;
        }
    }
}