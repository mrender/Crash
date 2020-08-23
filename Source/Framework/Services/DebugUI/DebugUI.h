#ifndef __GameDev2D__DebugUI__
#define __GameDev2D__DebugUI__

#include "../../Events/EventHandler.h"
#include "../../Graphics/Color.h"
#include "../../Math/Vector2.h"
#include "../../Math/Matrix.h"
#include <functional>
#include <vector>


namespace GameDev2D
{
    //Constants
    const Color DEBUG_TEXT_COLOR = Color::WhiteColor();
    const float DEBUG_TEXT_SPACING = 4.0f;

    //DebugUI class, will display the Game's FPS and other relevant information on screen. You may
    //also register a function pointer for most datatype and it will display it on screen as well
    class DebugUI : public EventHandler
    {
    public:
        DebugUI();
        ~DebugUI();
        
        //The HandleEvent is used to notify the DebugUI class of various GameDev2D events
        void HandleEvent(Event* event);

        //Draws the debug text to the screen
        void Draw();
        
        //Methods to watch a value
        void WatchUnsignedLongLong(std::function<unsigned long long()> callbackMethod, bool format);  //Can be formatted for memory output
        void WatchUnsignedInt(std::function<unsigned int()> callbackMethod);
        void WatchInt(std::function<int()> callbackMethod);
        void WatchFloat(std::function<float()> callbackMethod);
        void WatchDouble(std::function<double()> callbackMethod);
        void WatchVector2(std::function<Vector2()> callbackMethod);
        void WatchMatrix(std::function<Matrix()> callbackMethod);

    private:
        struct UnsignedLongLongCallback
        {
            UnsignedLongLongCallback(std::function<unsigned long long()> callback, bool format) :
                callback(callback),
                format(format)
            {

            }

            std::function<unsigned long long()> callback;
            bool format;
        };

        //Member variables
        std::vector<UnsignedLongLongCallback> m_UnsignedLongLongCallbacks;
        std::vector<std::function<unsigned int()>> m_UnsignedIntCallbacks;
        std::vector<std::function<int()>> m_IntCallbacks;
        std::vector<std::function<float()>> m_FloatCallbacks;
        std::vector<std::function<double()>> m_DoubleCallbacks;
        std::vector<std::function<Vector2()>> m_Vector2Callbacks;
        std::vector<std::function<Matrix()>> m_MatrixCallbacks;
    };
}

#endif
