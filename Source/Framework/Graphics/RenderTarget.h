#ifndef __GameDev2D__RenderTarget__
#define __GameDev2D__RenderTarget__

#include "GraphicTypes.h"
#include "Color.h"
#include <string>


namespace GameDev2D
{
    //Forward declarations
    class Texture;

    //The RenderTarget class allows you to render to the contents of another Texture
    class RenderTarget
    {
    public:
        //Creates the RenderTarget with the supplied Texture you want to render to
        RenderTarget(Texture* texture);
		RenderTarget(const Vector2& offset, Texture* texture);
        ~RenderTarget();

        //Call Begin() before you want to render
        void Begin(bool shouldClear = true);

        //Call End() when you are done rendering
        void End();

        //Returns the width and height of the RenderTarget
        unsigned int GetWidth();
        unsigned int GetHeight();

        //Sets the clear color for the RenderTarget
        void SetClearColor(Color clearColor);

        //Returns the clear color for the RenderTarget
        Color GetClearColor();

        //Saves the contents of the framebuffer to a png image at the path
        void SaveFrameBufferToFile(const std::string& aPath);

    private:
        //Member variables
        Texture* m_Texture;
        Color m_ClearColor;
        Color m_PreviousClearColor;
		Vector2 m_CameraOffset;
        unsigned int m_FrameBuffer;
    };
}

#endif 
