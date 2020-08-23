#ifndef __GameDev2D__Texture__
#define __GameDev2D__Texture__

#include "GraphicTypes.h"


namespace GameDev2D
{
    class Texture
    {
    public:
        //Creates a Texture object using the image data
        Texture(const ImageData& imageData);
        Texture(Texture& aTexture);
        Texture();
        ~Texture();

        //Returns the width of the texture
        unsigned int GetWidth();

        //Returns the height of the texture
        unsigned int GetHeight();
        
        //Returns the Texture's PixelFormat
        PixelFormat GetPixelFormat();
        
        //Returns the id of the texture
        unsigned int GetId();

        //Set (or update) the contents of the Texture
        void SetData(const ImageData& imageData);

    private:
        //Member variables
        PixelFormat m_Format;
        unsigned int m_Width;
        unsigned int m_Height;
        unsigned int m_Id;
    };
}

#endif
