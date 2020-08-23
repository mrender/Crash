#ifndef __GameDev2D__AnimatedSprite__
#define __GameDev2D__AnimatedSprite__

#include "SpriteAtlas.h"
#include "../Events/EventHandler.h"
#include <string>
#include <vector>


namespace GameDev2D
{
    class AnimatedSprite : public SpriteAtlas
    {
    public:
        //Creates a AnimatedSprite for the given filename, all the frames in the atlas will 
        //be stored, you must specify which frames are to be used in the animation using the 
        //AddFrame method below, by supplying the atlas key for each frame
        AnimatedSprite(const std::string& filename);
        ~AnimatedSprite();

        //Handles Update events so that the AnimatedSprite times each frame properly
        void HandleEvent(Event* event);
            
        //Adds a frame to the back of the animation, the frame must be in
        //the spriteAtlas that was loaded on creation
        void AddFrame(const std::string& atlasKey);

		//Removes all the frames in the animation
		void RemoveAllFrames();

        //Returns the frame count
        unsigned long GetFrameCount();

        //Returns the frame index
        int GetFrameIndex();

        //Sets the frame index
        void SetFrameIndex(unsigned int frameIndex);

        //Returns the frame speed of the sprite, must be animated
        float GetFrameSpeed();

        //Set the frame speed of an animated sprite
        void SetFrameSpeed(float frameSpeed);

        //Returns wether this Sprite loops, must be animated
        bool DoesLoop();

        //Sets if the Sprite loops, must be animated
        void SetDoesLoop(bool doesLoop);

		//Pauses an animation
		void Pause();

		//Resumes a paused animation
		void Resume();

    private:
        //Member variables
        std::vector<std::string> m_Frames;
        unsigned int m_FrameIndex;
        float m_FrameSpeed;
        double m_ElapsedTime;
        bool m_DoesLoop;
		bool m_Pause;
    };
}

#endif