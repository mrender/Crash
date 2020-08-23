#ifndef __GameDev2D__Graphics__
#define __GameDev2D__Graphics__

#include "../../Graphics/GraphicTypes.h"
#include "../../Graphics/Color.h"
#include "../../Graphics/RenderTarget.h"
#include "../../Graphics/Shader.h"
#include "../../Graphics/VertexData.h"
#include "../../Math/Matrix.h"
#include "../../Math/Rotation.h"


namespace GameDev2D
{
    //Forward declarations
    class Camera;
    class SpriteFont;
    class SpriteBatch;

    //
    class Graphics
    {
    public:
        Graphics();
        ~Graphics();
        
        //Returns the OpenGL API version
        unsigned int GetApiVersion();

        //Returns the GLSL version
        unsigned int GetShadingLanguageVersion();

        //Clears the currently bound RenderTarget's back buffer
        void Clear();

        //Draws a pre-loaded Texture to the screen at the supplied position and angle
        void DrawTexture(Texture* texture, Vector2 position, Rotation angle, float alpha = 1.0f);

        //Draws a string using a supplied SpriteFont to the screen at the supplied position and color
        void DrawString(FontData* fontData, const std::string& text, Vector2 position, Color color);

        //Draw a Rectangle to the screen at the supplied position and size, the Color can be set, and can be filled in
        void DrawRectangle(Vector2 position, Vector2 size, Rotation angle, Vector2 anchor, Color color, bool isFilled);

        //Draw a Circle to the screen at the supplied position and radius, the Color can be set, and can be filled in
        void DrawCircle(Vector2 position, float radius, Vector2 anchor, Color color, bool isFilled);

        //Draw a Line to the screen at the supplied endpoints, the Color can be set
        void DrawLine(Vector2 startPoint, Vector2 endPoint, Color color);

        //Call the OpenGL DrawArrays function
        void DrawArrays(RenderMode renderMode, int first, int count);

        //Call the OpenGL DrawElements function
        void DrawElements(RenderMode aRenderMode, int aCount, GraphicType aType, const void* aIndices);

        //Returns the active camera's projection and view matrices
        Matrix GetProjectionMatrix();
        Matrix GetViewMatrix();

        //Returns the active  Camera
        Camera* GetActiveCamera();

		//
		void PushCamera(const Camera& camera);

		//
		void PopCamera();

        //Sets the OpenGL viewport
        void SetViewport(const Viewport& viewport);

        //Enables and disables blending
        void EnableBlending(BlendingMode aBlendingMode);
        void DisableBlending();

        //Sets the clear color (the clear color is the color that clears the back buffer)
        void SetClearColor(Color clearColor);

        //Returns the clear color (the clear color is the color that clears the back buffer)
        Color GetClearColor();

        //Push a clipping rect onto the clipping stack
        void PushScissorClip(float x, float y, float width, float height);
        
        //Pops the last clipping rect off the clipping stack
        void PopScissorClip();

        //Updates the OpenGL scissor with the current scissor value in the scissor stack, if 
        //there is no value, OpenGL scissor is disabled
        void ResetScissorClip();

        //Bind the open gl texture id associated with the Texture object
        void BindTexture(Texture* texture);

        //Returns the currently bound texture id
        unsigned int GetBoundTextureId();

        //Generates an OpenGL Texture id and returns it
        unsigned int GenerateTexture();

        //Deletes an OpenGL Texture object (via the id)
        void DeleteTexture(Texture* texture);

        //
        void SetTextureData(Texture* texture, const ImageData& imageData);

        //
        void SetTextureWrap(Texture* texture, Wrap wrap);

        //Returns the id of the currently bound framebuffer
        unsigned int GetBoundFrameBufferId();

        //
        void BindFrameBuffer(unsigned int frameBuffer);

        //
        void GenerateFrameBuffer(unsigned int* frameBuffer);

        //Delete's an OpenGL framebuffer
        void DeleteFrameBuffer(unsigned int* frameBuffer);

        //Set the framebuffer storage for a specific texture id
        void SetFrameBufferStorage(unsigned int frameBuffer, unsigned int textureId);

        //
        void CopyFrameBufferContents(unsigned int frameBuffer, PixelFormat format, unsigned int width, unsigned int height, unsigned char* buffer);

        //Binds an open gl vertex array
        void BindVertexArray(unsigned int vertexArray);
        
        //Returns the currently bound vertex array
        unsigned int GetBoundVertexArray();

        //
        void GenerateVertexArray(unsigned int* vertexArray);

        //
        void DeleteVertexArray(unsigned int* vertexArray);

        //
        void BindDataBuffer(BufferType bufferType, unsigned int dataBuffer);

        //
        unsigned int GetBoundDataBuffer();

        //
        void GenerateDataBuffer(unsigned int* vertexBuffer);

        //
        void DeleteDataBuffer(unsigned int* vertexBuffer);

        //
        void InitializeDataBuffer(unsigned int dataBuffer, BufferType bufferType, long long size, const void* data, BufferUsage usage);

        //
        void UpdateDataBuffer(unsigned int dataBuffer, BufferType bufferType, long long offset, long long size, const void* data);

        //
        void SetVertexAttributePointer(unsigned int vertexBuffer, int attributeIndex, int size, int stride, const void * offset);

        //
        unsigned int CreateShaderProgram();

        //
        void DeleteShaderProgram(unsigned int* program);

        //
        unsigned int CreateShader(ShaderType shaderType);

        //
        void DeleteShader(unsigned int* shader);

        //
        bool CompileShader(unsigned int shader, const string& source);

        //
        void AttachShader(unsigned int program, unsigned int shader);

        //
        void DetachShader(unsigned int program, unsigned int shader);

        //
        bool LinkShaderProgram(unsigned int program);

        //
        bool ValidateShaderProgram(unsigned int program);

        //
        void UseShaderProgram(unsigned int program);

        //
        int GetUniformLocation(unsigned int program, const string& uniform);

        //
        int GetAttributeLocation(unsigned int program, const string& attribute);

        //
        void SetUniformFloat(unsigned int program, int location, float value);
        void SetUniformFloat(unsigned int program, int location, float value1, float value2);
        void SetUniformFloat(unsigned int program, int location, float value1, float value2, float value3);
        void SetUniformFloat(unsigned int program, int location, float value1, float value2, float value3, float value4);

        //
        void SetUniformInt(unsigned int program, int location, int value);
        void SetUniformInt(unsigned int program, int location, int value1, int value2);
        void SetUniformInt(unsigned int program, int location, int value1, int value2, int value3);
        void SetUniformInt(unsigned int program, int location, int value1, int value2, int value3, int value4);

        //
        void SetUniformVector2(unsigned int program, int location, const Vector2& value);

        //
        void SetUniformMatrix(unsigned int program, int location, const Matrix& value);

        //Converts the GraphicType data type into the OpenGL constant
        unsigned int GetOpenGLGraphicType(GraphicType aType);
       
        //Converts the PixelFormat::PixelLayout data type into the OpenGL constant
        unsigned int GetOpenGLPixelLayout(PixelFormat::PixelLayout aLayout);

        //Converts the PixelFormat::PixelSize data type into the OpenGL constant
        unsigned int GetOpenGLPixelSize(PixelFormat::PixelSize aSize);

        //Converts the BlendingFactor data type into the OpenGL constant
        unsigned int GetOpenGLBlendingFactor(BlendingFactor blendingFactor);

        //Converts the Wrap::WrapMode data type into the OpenGL constant
        int GetOpenGLWrapMode(Wrap::WrapMode wrapMode);

        //Converts the RenderMode data type into the OpenGL constant
        unsigned int GetOpenGLRenderMode(RenderMode renderMode);

        //Converts the BufferUsage data type into the OpenGL constant
        unsigned int GetOpenGLBufferUsage(BufferUsage dataBufferUsage);

        //Converts the BufferType data type into the OpenGL constant
        unsigned int GetOpenGLBufferType(BufferType dataBufferType);

        //Converts the ShaderType data type into the OpenGL constant
        unsigned int GetOpenGLShaderType(ShaderType shaderType);
        
        //Struct to hold onto Graphics stats
        struct Stats
        {
            Stats() :
                textureMemory(0)
            {
            }

            unsigned long long textureMemory;
        };

        //Returns the Graphics stats
        unsigned long long GetAllocatedTextureMemory();

    private:
        //Member variables
        VertexData* m_TexturedVertexData;
        VertexData* m_PolygonVertexData;
        SpriteBatch* m_SpriteBatch;  //Used for Text rendering
		std::vector<Camera> m_CameraStack;
        Color m_ClearColor;
        unsigned int m_BoundShaderProgram;
        unsigned int m_BoundTextureId;
        unsigned int m_BoundFrameBufferId;
        unsigned int m_BoundVertexArray;
        unsigned int m_BoundDataBuffer;
        std::vector<Rect> m_ScissorStack; 
        Stats m_Stats;
    };
}

#endif
