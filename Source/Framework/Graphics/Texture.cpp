#include "Texture.h"
#include "../Services/Services.h"
#include "../Services/Graphics/Graphics.h"


namespace GameDev2D
{
    Texture::Texture(const ImageData& aImageData) :
        m_Format(PixelFormat()),
        m_Width(0),
        m_Height(0),
        m_Id(0)
    {
        //Generate an id for the Texture
        m_Id = Services::GetGraphics()->GenerateTexture();

        //Set the imageData
        SetData(aImageData);
    }
    
    Texture::Texture(Texture& aTexture) :
        m_Format(aTexture.m_Format),
        m_Width(aTexture.m_Width),
        m_Height(aTexture.m_Height),
        m_Id(aTexture.m_Id)
    {
        //Clears the m_Id variable to zero, to prevent the OpenGL texture object from being deleted
        aTexture.m_Id = 0;
    }

    Texture::Texture() :
        m_Format(PixelFormat()),
        m_Width(0),
        m_Height(0),
        m_Id(0)
    {
        //Generate an id for the Texture
        m_Id = Services::GetGraphics()->GenerateTexture();
    }

    Texture::~Texture()
    {
        //Delete the texture from the GPU
        if(m_Id != 0)
        {
            Services::GetGraphics()->DeleteTexture(this);
            m_Id = 0;
        }
    }
    
    PixelFormat Texture::GetPixelFormat()
    {
        return m_Format;
    }

    unsigned int Texture::GetWidth()
    {
        return m_Width;
    }

    unsigned int Texture::GetHeight()
    {
        return m_Height;
    }

    unsigned int Texture::GetId()
    {
        return m_Id;
    }

    void Texture::SetData(const ImageData& aImageData)
    {
        //The Texture id can't be zero
        assert(m_Id != 0);

        //Pass the TextureData onto the graphics card
        Services::GetGraphics()->SetTextureData(this, aImageData);

        //Initialize the Texture members
        m_Width = aImageData.width;
        m_Height = aImageData.height;
        m_Format = aImageData.format;
    }
}
