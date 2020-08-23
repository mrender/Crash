#ifndef GameDev2D_Text_h
#define GameDev2D_Text_h

#include <string>


namespace GameDev2D
{
    class Text
    {
    public:
        static unsigned int NumberOfLines(const std::string& text);
        static void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);

        static std::string FormatBytes(unsigned long long bytes);
    };
}

#endif