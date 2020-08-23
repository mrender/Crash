#pragma once

namespace GameDev2D
{
    //Forward declaration
    struct ImageData;

    //A class that provides conveniance methods to load PNG image data
    class Png
    {
    public:
        static bool LoadFromPath(const std::string& path, ImageData** imageData);
        static bool SaveToPath(const std::string& path, ImageData* imageData);
    };
}
