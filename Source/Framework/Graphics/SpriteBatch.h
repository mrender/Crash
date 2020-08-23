#ifndef __GameDev2D__SpriteBatch__
#define __GameDev2D__SpriteBatch__

#include "GraphicTypes.h"
#include "Color.h"
#include "VertexData.h"
#include "../Math/Vector2.h"
#include "../Math/Matrix.h"


namespace GameDev2D
{
    //Local constants
    const unsigned int SPRITE_BATCH_COUNT = 300;

    //Forward declarations
    class Font;
    class Shader;
    class Sprite;
    class Texture;

    //SpriteBatch is used to batch sprite rendering together to save draw calls
    class SpriteBatch
    {
    public:
        SpriteBatch();
        ~SpriteBatch();

        //Called when the SpriteBatch should begin it's rendering
        void Begin();

        //Called when the SpriteBatch is done rendering, will flush the contents of the Batch
        void End();

        //Texture drawing methods
        void Draw(Texture* texture, Vector2 position, float angle, Vector2 scale = Vector2(1.0f, 1.0f));
        void Draw(Texture* texture, Vector2 position, float angle, Vector2 scale, Color color);
        void Draw(Texture* texture, Vector2 position, float angle, Vector2 scale, Color color, Vector2 anchor);
        void Draw(Texture* texture, Vector2 position, float angle, Vector2 scale, Color color, Vector2 anchor, Rect sourceFrame);

        //Texture drawing methods
        void Draw(Texture* texture, const Matrix& transformation);
        void Draw(Texture* texture, const Matrix& transformation, Color color);
        void Draw(Texture* texture, const Matrix& transformation, Color color, Vector2 anchor);
        void Draw(Texture* texture, const Matrix& transformation, Color color, Vector2 anchor, Rect sourceFrame);

        //Draws a Sprite
        void Draw(Sprite* sprite);

    private:
        //Draws the contents of the SpriteBatch
        void Flush();

        //Member variables
        Shader* m_Shader;
        VertexData* m_VertexData;
        Texture* m_CurrentTexture;
        bool m_Lock;
    };
}

#endif