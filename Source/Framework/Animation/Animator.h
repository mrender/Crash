#ifndef __GameDev2D__Animator__
#define __GameDev2D__Animator__

#include "Easing.h"
#include "../Events/EventHandler.h"
#include "../Graphics/Color.h"
#include "../Math/Vector2.h"
#include <functional>
#include <list>


namespace GameDev2D
{
    //Typedefs used to cleanup the code
    typedef std::function<void(float)> SetterFloatCallback;
    typedef std::function<float(void)> GetterFloatCallback;
    typedef std::function<void(Vector2)> SetterVector2Callback;
    typedef std::function<Vector2(void)> GetterVector2Callback;
    typedef std::function<void(Color)> SetterColorCallback;
    typedef std::function<Color(void)> GetterColorCallback;

    //Conveniance defines used to help cleanup the code
    #define MakeSetterCallback(object, method)  std::bind(&method, object, std::placeholders::_1)
    #define MakeGetterCallback(object, method)  std::bind(&method, object)
    #define MakeCallbacks(object, setter, getter) MakeSetterCallback(object, setter), MakeGetterCallback(object, getter)

    //The Base Animator class for AnimatorFloat and AnimatorVector2, primarily to avoid repeating code 
    class Animator : public EventHandler
    {
    public:
        Animator();
        virtual ~Animator();

        //Used to handle the Update event and apply the animation to the keyframes
        virtual void HandleEvent(Event* event);

        //Starts the animation
        virtual void Start();

        //Stops the animation
        virtual void Stop();

        //Returns wether the animation is running or not
        virtual bool IsAnimating();

    protected:
        //Must be implemented in inheriting classes, used to update the animation logic
        virtual void Update(double delta) = 0;

        //Must be implemented in inheriting classes, used to initalize the next keyframe
        virtual void NextKeyFrame() = 0;

        //Member variables
        double m_Time;
        bool m_IsAnimating;
    };


    //AnimationFloat class is used to animate float values using methods in an object 
    //(it technically doesn't have to be in an object though, read up on std::function 
    //to learn more about std::bind and how to hook up functions. Use the conveniance
    //define statement above to simplify function declarations.
    //
    // ***
    // IT IS VERY IMPORTANT TO ENSURE THE OBJECTS ANIMATING DOESN'T GO OUT OF SCOPE BEFORE  
    // THE ANIMATORS DO SINCE THE ANIMATORS HOLDS A REFERENCE TO THE OBJECT. IF THE OBJECT 
    // DOES IT WILL RESULT IN A CRASH
    // ***
    class AnimatorFloat : public Animator
    {
    public:
        AnimatorFloat(SetterFloatCallback setterCallback, GetterFloatCallback getterCallback);
        AnimatorFloat(float* reference);
        ~AnimatorFloat();

        //Adds a new keyframe to the list, a keyframe consists of the value to animate to, 
        //the duration that animation should take and the easing method to use
        void AddKeyframe(float value, double duration, EasingFunction easingFunction = Linear::Interpolation);

    private:
        //Updates the animation logic
        void Update(double delta);

        //Initalizes the next keyframe
        void NextKeyFrame();

        //Internal struct to manage Keyframe logic
        struct Keyframe
        {
            Keyframe(float value, double duration, EasingFunction easingFunction) :
                easingFunction(easingFunction),
                duration(duration),
                value(value)
            {
            }

            EasingFunction easingFunction;
            double duration;
            float value;
        };

        //Member variables
        std::list<Keyframe> m_Keyframes;
        SetterFloatCallback m_SetterCallback;
        GetterFloatCallback m_GetterCallback;
        float* m_Reference;
        float m_Start;
        float m_End;
    };


    // AnimatorVector2 class is used to animate float values using methods in an object 
    // (it technically doesn't have to be in an object though, read up on std::function 
    // to learn more about std::bind and how to hook up functions. Use the conveniance
    // define statement above to simplify function declarations.
    //
    // ***
    // IT IS VERY IMPORTANT TO ENSURE THE OBJECTS ANIMATING DOESN'T GO OUT OF SCOPE BEFORE  
    // THE ANIMATORS DO SINCE THE ANIMATORS HOLDS A REFERENCE TO THE OBJECT. IF THE OBJECT 
    // DOES IT WILL RESULT IN A CRASH
    // ***
    class AnimatorVector2 : public Animator
    {
    public:
        AnimatorVector2(SetterVector2Callback setterCallback, GetterVector2Callback getterCallback);
        AnimatorVector2(Vector2* reference);
        ~AnimatorVector2();

        //Adds a new keyframe to the list, a keyframe consists of the value to animate to, 
        //the duration that animation should take and the easing method to use
        void AddKeyframe(Vector2 value, double duration, EasingFunction easingFunction = Linear::Interpolation);

    private:
        //Updates the animation logic
        void Update(double delta);

        //Initalizes the next keyframe
        void NextKeyFrame();

        //Internal struct to manage Keyframe logic
        struct Keyframe
        {
            Keyframe(Vector2 value, double duration, EasingFunction easingFunction) :
                easingFunction(easingFunction),
                duration(duration),
                value(value)
            {
            }

            EasingFunction easingFunction;
            double duration;
            Vector2 value;
        };

        //Member variables
        std::list<Keyframe> m_Keyframes;
        SetterVector2Callback m_SetterCallback;
        GetterVector2Callback m_GetterCallback;
        Vector2* m_Reference;
        Vector2 m_Start;
        Vector2 m_End;
    };


    // AnimatorColor class is used to animate float values using methods in an object 
    // (it technically doesn't have to be in an object though, read up on std::function 
    // to learn more about std::bind and how to hook up functions. Use the conveniance
    // define statement above to simplify function declarations.
    //
    // ***
    // IT IS VERY IMPORTANT TO ENSURE THE OBJECTS ANIMATING DOESN'T GO OUT OF SCOPE BEFORE  
    // THE ANIMATORS DO SINCE THE ANIMATORS HOLDS A REFERENCE TO THE OBJECT. IF THE OBJECT 
    // DOES IT WILL RESULT IN A CRASH
    // ***
    class AnimatorColor : public Animator
    {
    public:
        AnimatorColor(SetterColorCallback setterCallback, GetterColorCallback getterCallback);
        AnimatorColor(Color* reference);
        ~AnimatorColor();

        //Adds a new keyframe to the list, a keyframe consists of the value to animate to, 
        //the duration that animation should take and the easing method to use
        void AddKeyframe(Color value, double duration, EasingFunction easingFunction = Linear::Interpolation);

    private:
        //Updates the animation logic
        void Update(double delta);

        //Initalizes the next keyframe
        void NextKeyFrame();

        //Internal struct to manage Keyframe logic
        struct Keyframe
        {
            Keyframe(Color value, double duration, EasingFunction easingFunction) :
                easingFunction(easingFunction),
                duration(duration),
                value(value)
            {
                if (value.r > 1.0f)
                {

                }

            }

            EasingFunction easingFunction;
            double duration;
            Color value;
        };

        //Member variables
        std::list<Keyframe> m_Keyframes;
        SetterColorCallback m_SetterCallback;
        GetterColorCallback m_GetterCallback;
        Color* m_Reference;
        Color m_Start;
        Color m_End;
    };
}

#endif