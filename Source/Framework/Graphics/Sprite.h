#ifndef __GameDev2D__Sprite__
#define __GameDev2D__Sprite__

#include "GraphicTypes.h"
#include "Texture.h"
#include "VertexData.h"
#include "../Core/Drawable.h"


namespace GameDev2D
{
    //Sprite constant
    const unsigned int SPRITE_VERTEX_SIZE = 4;  //2 position, 2 uv
    const unsigned int SPRITE_VERTEX_COUNT = 4;

    //The Sprite class is used for Draw a Texture to the screen, it has it's own 
    //Draw() method which must be called inside a Draw function. It inherits for Drawable, 
    //which inherits from Transformable, meaning in can have its position, rotation, scale, 
    //anchor point, color and Shader modified
    class Sprite : public Drawable, public EventHandler
    {
    public:
        //Creates a Sprite using a reference to an already loaded Texture object, 
        //you are responsible for unloading the Texture
        Sprite(Texture* texture);

		//Creates a Sprite using a reference to an already loaded Texture object, 
		//you are responsible for unloading the Texture
		Sprite(const std::string& filename);

        //Destructor
        virtual ~Sprite();

        //Draws the Sprite
        virtual void Draw();
        
        //Returns the width and height of the Sprite
        virtual float GetWidth();
        virtual float GetHeight();

        //Sets the wrap mode of the Sprite
        void SetWrap(Wrap::WrapMode wrapModeS, Wrap::WrapMode wrapModeT);
        void SetWrap(Wrap wrap);

        //Returns the wrap mode of the Sprite
        Wrap GetWrap();

        //Returns the pixel format of the Sprite
        PixelFormat GetPixelFormat();
        
        //Sets the frame of the Texture to render
        void SetFrame(Rect frame);
        
        //Returns the frame of the Texture to render
        Rect GetFrame();
        
        //Set's the Sprite's Texture object
        void SetTexture(Texture* texture);

        //Called from the TextureManager
        Texture* GetTexture() const;

		//Handle system events
		void HandleEvent(Event* event);
        
    protected:
        //Member variables
        Texture* m_Texture;
        VertexData* m_VertexData;
        Rect m_Frame;
        Wrap m_Wrap;
    };
}

#endif
