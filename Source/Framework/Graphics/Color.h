#ifndef __GameDev2D__Color__
#define __GameDev2D__Color__

namespace GameDev2D
{
    //Basic color class to manage RGBA color data. Color components are stored as 
    //float variables and have a range of 0.0f - 1.0f there is nothing to stop 
    //you from breaking these ranges, so be aware of that
    struct Color
    {
        //Create the color white
        Color();

        //Copies another Color in the newly created Color
        Color(const Color& color);
        
        //float range: 0.0f - 1.0f
        Color(float red, float green, float blue, float alpha = 1.0f);
        
        //unsigned char range: 0 - 255
        Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        
        //Hexadecimal RGBA
        Color(unsigned int color);

        //float range: 0.0f - 1.0f
        void Set(float red, float green, float blue, float alpha);
        
        //unsigned char range: 0 - 255
        void Set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
        
        //hexadecimal RGBA
        void Set(unsigned int color);

        //Overloaded operators
        bool operator==(const Color& color) const;
        bool operator!=(const Color& color) const;
        
        //Inverts the color
        void InvertColor();
        
        //Logs the color
        void Log();

        //Returns wether a color has valid color values
        static bool IsValid(Color color);

        //Static color presets
        static Color ClearColor();
        static Color BlackColor(float alpha = 1.0f);
        static Color WhiteColor(float alpha = 1.0f);
        static Color RedColor(float alpha = 1.0f);
        static Color GreenColor(float alpha = 1.0f);
        static Color BlueColor(float alpha = 1.0f);
        static Color YellowColor(float alpha = 1.0f);
        static Color CyanColor(float alpha = 1.0f);
        static Color OrangeColor(float alpha = 1.0f);
        static Color GrayColor(float alpha = 1.0f);
        static Color DarkGrayColor(float alpha = 1.0f);
        
        static Color CharcoalColor(float alpha = 1.0f);
        static Color LicoriceColor(float alpha = 1.0f);
        static Color OnyxColor(float alpha = 1.0f);
        static Color OuterSpaceBlackColor(float alpha = 1.0f);
        
        static Color CrimsonRedColor(float alpha = 1.0f);
        static Color FireEngineRedColor(float alpha = 1.0f);
        static Color BarnRedColor(float alpha = 1.0f);
        
        static Color AppleGreenColor(float alpha = 1.0f);
        static Color ForestGreenColor(float alpha = 1.0f);
        static Color ShamrockGreenColor(float alpha = 1.0f);
        static Color LimeColor(float alpha = 1.0f);
        
        static Color CornflowerBlueColor(float alpha = 1.0f);
        static Color EgyptianBlueColor(float alpha = 1.0f);
        static Color PowderBlueColor(float alpha = 1.0f);
        
        static Color MellowYellowColor(float alpha = 1.0f);
        static Color RoyalYellowColor(float alpha = 1.0f);
        static Color SafetyYellowColor(float alpha = 1.0f);
        
        static Color AntiqueWhiteColor(float alpha = 1.0f);
        static Color FloralWhiteColor(float alpha = 1.0f);
        static Color GhostWhiteColor(float alpha = 1.0f);
        
        static Color Random(float alpha = 1.0f);

        //Member variables
        float r;
        float g;
        float b;
        float a;
    };
}

#endif
