#ifndef __GameDev2D__Easing__
#define __GameDev2D__Easing__

namespace GameDev2D
{
    //Easing function pointer
    typedef float(*EasingFunction)(float value);

    //Linear ease
    class Linear
    {
    public:
        static float Interpolation(float value);
    };


    //Quadratic ease (in, out, inOut)
    class Quadratic
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };


    //Cubic ease (in, out, inOut)
    class Cubic
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };


    //Quartic ease (in, out, inOut)
    class Quartic
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };


    //Quintic ease (in, out, inOut)
    class Quintic
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };


    //Sinusoidal ease (in, out, inOut)
    class Sinusoidal
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };


    //Exponential ease (in, out, inOut)
    class Exponential
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };


    //Circular ease (in, out, inOut)
    class Circular
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };


    //Elastic ease (in, out, inOut)
    class Elastic
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };


    //Back ease (in, out, inOut)
    class Back
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };


    //Bounce ease (in, out, inOut)
    class Bounce
    {
    public:
        static float In(float value);
        static float Out(float value);
        static float InOut(float value);
    };
}
#endif