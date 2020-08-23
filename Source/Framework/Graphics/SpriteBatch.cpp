#include "SpriteBatch.h"
#include "Shader.h"
#include "Sprite.h"
#include "Texture.h"
#include "../Math/Math.h"
#include "../Services/Services.h"
#include <assert.h>


namespace GameDev2D
{
    SpriteBatch::SpriteBatch() : 
        m_VertexData(nullptr),
        m_CurrentTexture(nullptr),
        m_Lock(false)
    {
        //Initialize the Shader
        m_Shader = Services::GetResourceManager()->GetShader(PASSTHROUGH_SPRITEBATCH_SHADER_KEY);

        //Create the VertexData object
        m_VertexData = new VertexData();

        //Initialize the SpriteBatch's vertex DataBufferDescriptor, it describes
        //how the individual 'elements' will be stored in the DataBuffer
        VertexBufferDescriptor vertexDescriptor;
        vertexDescriptor.size = 8;
        vertexDescriptor.capacity = SPRITE_BATCH_COUNT;
        vertexDescriptor.usage = BufferUsage_DynamicDraw;
        vertexDescriptor.attributeIndex = { m_Shader->GetAttributeLocation("in_vertices"), m_Shader->GetAttributeLocation("in_uv"), m_Shader->GetAttributeLocation("in_color") };
        vertexDescriptor.attributeSize = { 2, 2, 4 };

        //Create the vertex buffer
        m_VertexData->CreateBuffer(vertexDescriptor);

        //Initialize the SpriteBatch's  DataBufferDescriptor, it describes
        //how the individual 'elements' will be stored in the DataBuffer
        IndexBufferDescriptor indexDescriptor;
        indexDescriptor.size = 6;
        indexDescriptor.capacity = SPRITE_BATCH_COUNT;
        indexDescriptor.usage = BufferUsage_StaticDraw;

        //Initialize the indicies
        std::vector<unsigned short> indices;
        for (unsigned int i = 0; i < SPRITE_BATCH_COUNT; i++)
        {
            indices.push_back(i * 4 + 1);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 0);
            indices.push_back(i * 4 + 0);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 3);
        }

        //Create the index buffer
        m_VertexData->CreateBuffer(indexDescriptor, &indices);
    }

    SpriteBatch::~SpriteBatch()
    {
        if (m_VertexData != nullptr)
        {
            delete m_VertexData;
            m_VertexData = nullptr;
        }
    }

    void SpriteBatch::Begin()
    {
        //If this assert is hit, you are trying to draw to a SpriteBatch that has already had Begin() called BUT not End()
        assert(m_Lock == false);

        //Lock the SpriteBatch
        m_Lock = true;
    }

    void SpriteBatch::End()
    {
        //Flush the vertex data (draw to the screen)
        Flush();

        //Unlock the SpriteBatch
        m_Lock = false;
    }

    void SpriteBatch::Draw(Texture* aTexture, Vector2 aPosition, float aAngle, Vector2 aScale)
    {
        Draw(aTexture, aPosition, aAngle, aScale, Color::WhiteColor(), Vector2(1.0f, 1.0f));
    }

    void SpriteBatch::Draw(Texture* aTexture, Vector2 aPosition, float aAngle, Vector2 aScale, Color aColor)
    {
        Draw(aTexture, aPosition, aAngle, aScale, aColor, Vector2(1.0f, 1.0f));
    }

    void SpriteBatch::Draw(Texture* aTexture, Vector2 aPosition, float aAngle, Vector2 aScale, Color aColor, Vector2 aAnchor)
    {
        Draw(aTexture, aPosition, aAngle, aScale, aColor, aAnchor, Rect(Vector2(0.0f, 0.0f), Vector2((float)aTexture->GetWidth(), (float)aTexture->GetHeight())));
    }

    void SpriteBatch::Draw(Texture* aTexture, Vector2 aPosition, float aAngle, Vector2 aScale, Color aColor, Vector2 aAnchor, Rect aSourceFrame)
    {
        //Draw the texture
        Draw(aTexture, Matrix::Make(aPosition, aAngle, aScale), aColor, aAnchor, aSourceFrame);
    }

    void SpriteBatch::Draw(Texture* aTexture, const Matrix& aTransformation)
    {
        Draw(aTexture, aTransformation, Color::WhiteColor(), Vector2(0.0f, 0.0f));
    }

    void SpriteBatch::Draw(Texture* aTexture, const Matrix& aTransformation, Color aColor)
    {
        Draw(aTexture, aTransformation, aColor, Vector2(0.0f, 0.0f));
    }

    void SpriteBatch::Draw(Texture* aTexture, const Matrix& aTransformation, Color aColor, Vector2 aAnchor)
    {
        Draw(aTexture, aTransformation, aColor, aAnchor, Rect(Vector2(0.0f, 0.0f), Vector2((float)aTexture->GetWidth(), (float)aTexture->GetHeight())));
    }

    void SpriteBatch::Draw(Texture* aTexture, const Matrix& aTransformation, Color aColor, Vector2 aAnchor, Rect aSourceFrame)
    {
        //If the SpriteBatch has reacher the maximum vertices, flush the data
        if (m_VertexData->GetVertexBuffer()->GetCount() == SPRITE_BATCH_COUNT)
        {
            Flush();
        }

        //Safety check the texture
        if (aTexture != nullptr)
        {
            //If the Texture is different, Flush the vertex data
            if (m_CurrentTexture != aTexture)
            {
                Flush();
            }

            //Set the current Texture
            m_CurrentTexture = aTexture;
        }

        //Get the texture and source sizes
        Vector2 textureSize = Vector2((float)aTexture->GetWidth(), (float)aTexture->GetHeight());
        Vector2 sourceSize = aSourceFrame.size;

        //Build the UV Coordinates
        float u1 = 0.0f;
        float u2 = 1.0f;
        float v1 = 0.0f;
        float v2 = 1.0f;

        //Recalculate the UVs if the texture size and source size differ
        if (textureSize != sourceSize)
        {
            u1 = (float)aSourceFrame.origin.x / (float)textureSize.x;
            u2 = (float)(aSourceFrame.origin.x + aSourceFrame.size.x) / (float)textureSize.x;
            v1 = 1.0f - (((float)aSourceFrame.origin.y + aSourceFrame.size.y) / (float)textureSize.y);
            v2 = 1.0f - (aSourceFrame.origin.y / (float)textureSize.y);
        }

        //Calculate the vertices
        Vector2 offsetA = Vector2(0.0f, sourceSize.y) - Vector2(aAnchor.x * sourceSize.x, aAnchor.y * sourceSize.y);
        Vector2 offsetB = Vector2(sourceSize.x, sourceSize.y) - Vector2(aAnchor.x * sourceSize.x, aAnchor.y * sourceSize.y);
        Vector2 offsetC = Vector2(sourceSize.x, 0.0f) - Vector2(aAnchor.x * sourceSize.x, aAnchor.y * sourceSize.y);
        Vector2 offsetD = Vector2(0.0f, 0.0f) - Vector2(aAnchor.x * sourceSize.x, aAnchor.y * sourceSize.y);

        //Apply a matrix transformation to each vertices
        Vector2 vertexA = aTransformation * offsetA;
        Vector2 vertexB = aTransformation * offsetB;
        Vector2 vertexC = aTransformation * offsetC;
        Vector2 vertexD = aTransformation * offsetD;

        //Add the vertices to the vertex buffer
        m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { vertexA.x, vertexA.y, u1, v2, aColor.r, aColor.g, aColor.b, aColor.a });  //x,y+h,u1,v2
        m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { vertexB.x, vertexB.y, u2, v2, aColor.r, aColor.g, aColor.b, aColor.a });  //x+w,y+h,u2,v2
        m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { vertexC.x, vertexC.y, u2, v1, aColor.r, aColor.g, aColor.b, aColor.a });  //x+w,y,u2,v1
        m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { vertexD.x, vertexD.y, u1, v1, aColor.r, aColor.g, aColor.b, aColor.a });  //x,y,u1,v1
    }

    void SpriteBatch::Draw(Sprite* aSprite)
    {
        if (aSprite != nullptr)
        {
            Draw(aSprite->GetTexture(), aSprite->GetTransformMatrix(), aSprite->GetColor(), aSprite->GetAnchor(), aSprite->GetFrame());
        }
    }

    void SpriteBatch::Flush()
    {
        //We can't draw anything if there isn't any vertices OR a texture set
        if (m_VertexData->GetVertexBuffer()->GetCount() == 0 || m_CurrentTexture == nullptr)
        {
            return;
        }

        //Bind the texture
        Services::GetGraphics()->BindTexture(m_CurrentTexture);

        //Cache the Graphics service
        Graphics* graphics = Services::GetGraphics();

        //Bind the vertex array object
        m_VertexData->PrepareForDraw();

        //Setup the shader uniforms
        m_Shader->SetUniformInt("uniform_texture", 0);
        m_Shader->SetUniformMatrix("uniform_model", Matrix::Identity());
        m_Shader->SetUniformMatrix("uniform_view", graphics->GetViewMatrix());
        m_Shader->SetUniformMatrix("uniform_projection", graphics->GetProjectionMatrix());

        //Validate the shader, return if it fails
        if (m_Shader->Validate() == false)
        {
            return;
        }

        //Bind the texture
        graphics->BindTexture(m_CurrentTexture);

        //Enable blending if the texture has an alpha channel
        if (m_CurrentTexture->GetPixelFormat().layout == PixelFormat::RGBA /*|| m_Color.a != 1.0f*/)
        {
            graphics->EnableBlending(BlendingMode());
        }

        //Draw the indices
        unsigned int sprites = m_VertexData->GetVertexBuffer()->GetCount() / 4;
        unsigned int count = sprites * m_VertexData->GetIndexBuffer()->GetSize();
        graphics->DrawElements(RenderMode_Triangles, count, GraphicType_UnsignedShort, 0);

        //Disable blending, if we did in fact have it enabled
        if (m_CurrentTexture->GetPixelFormat().layout == PixelFormat::RGBA /*|| m_Color.a != 1.0f*/)
        {
            graphics->DisableBlending();
        }

        //This unbind's the vertex array, so its not accidently modified
        m_VertexData->EndDraw();
        m_VertexData->GetVertexBuffer()->ClearVertices();
    }
}