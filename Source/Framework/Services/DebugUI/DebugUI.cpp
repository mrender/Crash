#include "DebugUI.h"
#include "../Services.h"
#include "../../Graphics/SpriteFont.h"
#include "../../Utils/Text/Text.h"
#include "../../Windows/Application.h"
#include <GameDev2D.h>
#include <sstream>


namespace GameDev2D
{
    DebugUI::DebugUI() : EventHandler()
    {
#if DEBUG_DRAW_FPS
        WatchUnsignedInt(std::bind(&GameLoop::GetFPS, Services::GetApplication()->GetGameLoop()));
#endif

#if DEBUG_DRAW_DELTA_TIME
        WatchDouble(std::bind(&GameLoop::GetDelta, Services::GetApplication()->GetGameLoop()));
#endif

#if DEBUG_DRAW_ELAPSED_TIME
        WatchDouble(std::bind(&GameLoop::GetElapsedTime, Services::GetApplication()->GetGameLoop()));
#endif

#if DEBUG_DRAW_ALLOCATED_TEXTURE_MEMORY
        WatchUnsignedLongLong(std::bind(&Graphics::GetAllocatedTextureMemory, Services::GetGraphics()), true);
#endif

#if DEBUG_DRAW_MOUSE_POSITION
		WatchVector2(std::bind(&Mouse::GetPosition, Services::GetInputManager()->GetMouse()));
#endif

		//WatchUnsignedLongLong(MyMemory_GetNumberOfBytesAllocated, true);
	//	WatchUnsignedLongLong(MyMemory_GetNumberOfMemoryAllocations, false);




#if DEBUG ||_DEBUG
        //Add an event listener callback for the Update event
        Services::GetApplication()->AddEventListener(this, DRAW_EVENT);
#endif
    }

    DebugUI::~DebugUI()
    {
#if DEBUG ||_DEBUG
        //Add an event listener callback for the Update event
        Services::GetApplication()->RemoveEventListener(this, DRAW_EVENT);
#endif
    }

    void DebugUI::HandleEvent(Event* aEvent)
    {
        if (aEvent != nullptr)
        {
            if (aEvent->GetEventCode() == DRAW_EVENT)
            {
                Draw();
            }
        }
    }
    
    void DebugUI::Draw()
    {
#if DEBUG
		Services::GetGraphics()->PushCamera(Camera(Viewport(GetScreenWidth(), GetScreenHeight())));

        FontData* fontData = Services::GetResourceManager()->GetDefaultFont();
        if (fontData != nullptr)
        {
            std::stringstream ss;
            const float spacer = DEBUG_TEXT_SPACING;
            const float width = static_cast<float>(Services::GetApplication()->GetWindow()->GetWidth());
            const float height = static_cast<float>(Services::GetApplication()->GetWindow()->GetHeight());
            Vector2 origin = Vector2(spacer, height - fontData->lineHeight - spacer);

            for (unsigned int i = 0; i < m_UnsignedIntCallbacks.size(); i++)
            {
                ss << m_UnsignedIntCallbacks.at(i)();
                Services::GetGraphics()->DrawString(fontData, ss.str(), origin, DEBUG_TEXT_COLOR);
                ss.str(""); //Clear the stringstream
                origin.y -= fontData->lineHeight + spacer;
            }

            for (unsigned int i = 0; i < m_IntCallbacks.size(); i++)
            {
                ss << m_IntCallbacks.at(i)();
                Services::GetGraphics()->DrawString(fontData, ss.str(), origin, DEBUG_TEXT_COLOR);
                ss.str(""); //Clear the stringstream
                origin.y -= fontData->lineHeight + spacer;
            }

            for (unsigned int i = 0; i < m_UnsignedLongLongCallbacks.size(); i++)
            {
                std::function<unsigned long long()> callback = m_UnsignedLongLongCallbacks.at(i).callback;
                bool format = m_UnsignedLongLongCallbacks.at(i).format;

                if (format == true)
                {
                    Services::GetGraphics()->DrawString(fontData, Text::FormatBytes(callback()), origin, DEBUG_TEXT_COLOR);
                }
                else
                {
                    ss << callback();
                    Services::GetGraphics()->DrawString(fontData, ss.str(), origin, DEBUG_TEXT_COLOR);
                    ss.str(""); //Clear the stringstream
                }

                origin.y -= fontData->lineHeight + spacer;

            }

            for (unsigned int i = 0; i < m_FloatCallbacks.size(); i++)
            {
                ss << m_FloatCallbacks.at(i)();
                Services::GetGraphics()->DrawString(fontData, ss.str(), origin, DEBUG_TEXT_COLOR);
                ss.str(""); //Clear the stringstream
                origin.y -= fontData->lineHeight + spacer;
            }

            for (unsigned int i = 0; i < m_DoubleCallbacks.size(); i++)
            {
                ss << m_DoubleCallbacks.at(i)();
                Services::GetGraphics()->DrawString(fontData, ss.str(), origin, DEBUG_TEXT_COLOR);
                ss.str(""); //Clear the stringstream
                origin.y -= fontData->lineHeight + spacer;
            }

            for (unsigned int i = 0; i < m_Vector2Callbacks.size(); i++)
            {
                Vector2 value = m_Vector2Callbacks.at(i)();
                ss << value.x << "," << value.y;
                Services::GetGraphics()->DrawString(fontData, ss.str(), origin, DEBUG_TEXT_COLOR);
                ss.str(""); //Clear the stringstream
                origin.y -= fontData->lineHeight + spacer;
            }

            for (unsigned int i = 0; i < m_MatrixCallbacks.size(); i++)
            {
                origin.y -= fontData->lineHeight * 3;
                Matrix value = m_MatrixCallbacks.at(i)();
                ss << value.m[0][0] << "," << value.m[0][1] << "," << value.m[0][2] << "," << value.m[0][3] << std::endl;
                ss << value.m[1][0] << "," << value.m[1][1] << "," << value.m[1][2] << "," << value.m[1][3] << std::endl;
                ss << value.m[2][0] << "," << value.m[2][1] << "," << value.m[2][2] << "," << value.m[2][3] << std::endl;
                ss << value.m[3][0] << "," << value.m[3][1] << "," << value.m[3][2] << "," << value.m[3][3];
                std::string matrixString = ss.str();
                Text::ReplaceStringInPlace(matrixString, "-0", "0");  //The mat4 can format 0 to be -0, clean up the string
                Services::GetGraphics()->DrawString(fontData, matrixString, origin, DEBUG_TEXT_COLOR);
                ss.str(""); //Clear the stringstream
                origin.y -= fontData->lineHeight + spacer;
            }
        }
		Services::GetGraphics()->PopCamera();
#endif
    }

    void DebugUI::WatchUnsignedLongLong(std::function<unsigned long long()> aCallbackMethod, bool aFormat)
    {
#if DEBUG ||_DEBUG
        m_UnsignedLongLongCallbacks.push_back(UnsignedLongLongCallback(aCallbackMethod, aFormat));
#endif
    }

    void DebugUI::WatchUnsignedInt(std::function<unsigned int()> aCallbackMethod)
    {
#if DEBUG ||_DEBUG
        m_UnsignedIntCallbacks.push_back(aCallbackMethod);
#endif
    }

    void DebugUI::WatchInt(std::function<int()> aCallbackMethod)
    {
#if DEBUG ||_DEBUG
        m_IntCallbacks.push_back(aCallbackMethod);
#endif
    }

    void DebugUI::WatchFloat(std::function<float()> aCallbackMethod)
    {
#if DEBUG ||_DEBUG
        m_FloatCallbacks.push_back(aCallbackMethod);
#endif
    }

    void DebugUI::WatchDouble(std::function<double()> aCallbackMethod)
    {
#if DEBUG ||_DEBUG
        m_DoubleCallbacks.push_back(aCallbackMethod);
#endif
    }

    void DebugUI::WatchVector2(std::function<Vector2()> aCallbackMethod)
    {
#if DEBUG ||_DEBUG
        m_Vector2Callbacks.push_back(aCallbackMethod);
#endif
    }

    void DebugUI::WatchMatrix(std::function<Matrix()> aCallbackMethod)
    {
#if DEBUG ||_DEBUG
        m_MatrixCallbacks.push_back(aCallbackMethod);
#endif
    }
}