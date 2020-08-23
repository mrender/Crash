#include "Text.h"
#include <iomanip>
#include <sstream>


namespace GameDev2D
{
    unsigned int Text::NumberOfLines(const std::string& aText)
    {
        unsigned int numberOfLines = 1;

        //Cycle through all the characters in the text string
        for (unsigned int i = 0; i < aText.length(); i++)
        {
            //Did we reach a new line?
            if (aText.at(i) == '\n')
            {
                numberOfLines++;
            }
        }

        return numberOfLines;
    }

    void Text::ReplaceStringInPlace(std::string& aSubject, const std::string& aSearch, const std::string& aReplace)
    {
        if (aSubject.length() > 0 && aSearch.length() > 0 && aReplace.length() > 0)
        {
            size_t pos = 0;
            while ((pos = aSubject.find(aSearch, pos)) != std::string::npos)
            {
                aSubject.replace(pos, aSearch.length(), aReplace);
                pos += aReplace.length();
            }
        }
    }

    std::string Text::FormatBytes(unsigned long long aBytes)
    {
        int unit = 1024;
        if (aBytes < unit)
        {
            return std::string(aBytes + " B");
        }

        int exp = (int)(logf((float)aBytes) / logf((float)unit));
        std::stringstream ss;
        ss << std::setprecision(1) << std::fixed << (float)aBytes / powf((float)unit, (float)exp) << std::string("KMGTPE").at(exp - 1) << "B";
        return ss.str();
    }
}