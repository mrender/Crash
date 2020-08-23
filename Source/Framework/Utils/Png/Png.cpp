#include "Png.h"
#include "../../Graphics/GraphicTypes.h"
#include <lodepng.h>


namespace GameDev2D
{
    bool Png::LoadFromPath(const std::string& aPath, ImageData** aImageData)
    {
        //Local variables
        std::vector<unsigned char> image; //the raw pixels
        unsigned int width = 0;
        unsigned int height = 0;

        //Decide the image
        unsigned error = lodepng::decode(image, width, height, aPath);

        //Safety check that there is actually image data
        if (width > 0 && height > 0)
        {
            //Allocate the image data
            const unsigned int bytesPerPixel = 4;
            size_t bufferSize = width * height * bytesPerPixel;
            size_t bytesPerRow = width * bytesPerPixel;
            unsigned char* buffer = new unsigned char[bufferSize];

            //The png image is ordered top to bottom, but OpenGL expect it bottom to top so the order or swapped
            for (unsigned int i = 0; i < height; i++)
            {
                memcpy(buffer + (bytesPerRow * (height - 1 - i)), &image[i*bytesPerRow], bytesPerRow);
            }

            //Set the image data
            PixelFormat pixelFormat = PixelFormat(PixelFormat::RGBA, PixelFormat::UnsignedByte);
            *aImageData = new ImageData(pixelFormat, width, height, buffer);
        }

        //If we got here, then everthing was successful
        return true;
    }

    bool Png::SaveToPath(const std::string& aPath, ImageData* aImageData)
    {
        unsigned error = lodepng::encode(aPath, aImageData->data, aImageData->width, aImageData->height);
        return error == 0;
    }
}