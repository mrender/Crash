#include "RenderTarget.h"
#include "Texture.h"
#include "Camera.h"
#include "../Services/Services.h"
#include "../Windows/Application.h"
#include "../Utils/Png/Png.h"
#include <assert.h>


namespace GameDev2D
{    
    RenderTarget::RenderTarget(Texture* aTexture) : 
        m_Texture(aTexture),
        m_ClearColor(Color::ClearColor()),
        m_PreviousClearColor(Color::WhiteColor()),
		m_CameraOffset(Vector2::Zero),
        m_FrameBuffer(0)
    {
        //Generate the framebuffer
        Services::GetGraphics()->GenerateFrameBuffer(&m_FrameBuffer);

        //Set the framebuffer storage to that of the texture
        Services::GetGraphics()->SetFrameBufferStorage(m_FrameBuffer, m_Texture->GetId());
    }

	RenderTarget::RenderTarget(const Vector2& aOffset, Texture* aTexture) :
		m_Texture(aTexture),
		m_ClearColor(Color::ClearColor()),
		m_PreviousClearColor(Color::WhiteColor()),
		m_CameraOffset(aOffset),
		m_FrameBuffer(0)
	{
		//Generate the framebuffer
		Services::GetGraphics()->GenerateFrameBuffer(&m_FrameBuffer);

		//Set the framebuffer storage to that of the texture
		Services::GetGraphics()->SetFrameBufferStorage(m_FrameBuffer, m_Texture->GetId());
	}

    RenderTarget::~RenderTarget()
    {
        //Destroy the framebuffer
        Services::GetGraphics()->DeleteFrameBuffer(&m_FrameBuffer);
    }

    void RenderTarget::Begin(bool aShouldClear)
    {
        //Ensure there isn't already a bound framebuffer
        assert(Services::GetGraphics()->GetBoundFrameBufferId() == 0);

        //Bind the framebuffer
        Services::GetGraphics()->BindFrameBuffer(m_FrameBuffer);

        //Push the viewport
		Services::GetGraphics()->PushCamera(Camera(Viewport((unsigned int)m_CameraOffset.x, (unsigned int)m_CameraOffset.y, GetWidth(), GetHeight()), false));

        //Push the clear color
        m_PreviousClearColor = Services::GetGraphics()->GetClearColor();
        Services::GetGraphics()->SetClearColor(GetClearColor());

		//Should the texture be cleared?
		if (aShouldClear == true)
		{
			//Lastly, clear the buffer
			Services::GetGraphics()->Clear();
		}
    }

    void RenderTarget::End()
    {
        //Unbind the framebuffer
        Services::GetGraphics()->BindFrameBuffer(0);

        //Pop the viewport size
		Services::GetGraphics()->PopCamera();

        //Pop the clear color
        Services::GetGraphics()->SetClearColor(m_PreviousClearColor);
    }

    unsigned int RenderTarget::GetWidth()
    {
        if (m_Texture != nullptr)
        {
            return (unsigned int)m_Texture->GetWidth();
        }

        return 0;
    }

    unsigned int RenderTarget::GetHeight()
    {
        if (m_Texture != nullptr)
        {
            return (unsigned int)m_Texture->GetHeight();
        }

        return 0;
    }

    void RenderTarget::SetClearColor(Color aClearColor)
    {
        m_ClearColor = aClearColor;
    }

    Color RenderTarget::GetClearColor()
    {
        return m_ClearColor;
    }

    void RenderTarget::SaveFrameBufferToFile(const string& aPath)
    {
        //Allocate the buffer
        const unsigned int bytesPerPixel = 4;
        size_t bufferSize = GetWidth() * GetHeight() * bytesPerPixel;
        unsigned char* buffer = new unsigned char[bufferSize];

        //Copy the contents of the framebuffer to the allocated buffer
        PixelFormat format = PixelFormat(PixelFormat::RGBA, PixelFormat::UnsignedByte);
        Services::GetGraphics()->CopyFrameBufferContents(m_FrameBuffer, format, GetWidth(), GetHeight(), buffer);

        //We need to swap the buffer
        unsigned char* flippedBuffer = new unsigned char[bufferSize];
        size_t bytesPerRow = GetWidth() * bytesPerPixel;

        //The png image is ordered top to bottom, but OpenGL expect it bottom to top so the order or swapped
        for (unsigned int i = 0; i < GetHeight(); i++)
        {
            memcpy(flippedBuffer + (bytesPerRow * (GetHeight() - 1 - i)), &buffer[i*bytesPerRow], bytesPerRow);
        }

        //Create a temporary texture data object to pass into the png saving method
        ImageData imageData(format, GetWidth(), GetHeight(), flippedBuffer);

        //Save the texture data to the path
        Png::SaveToPath(aPath, &imageData);

        //Delete the arrays
        if (buffer != nullptr)
        {
            delete[] buffer;
            buffer = nullptr;
        }

        if (flippedBuffer != nullptr)
        {
            delete[] flippedBuffer;
            flippedBuffer = nullptr;
        }
    }
}