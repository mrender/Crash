#include "Graphics.h"
#include "../Services.h"
#include "../../Core/Drawable.h"
#include "../../Debug/Log.h"
#include "../../Graphics/Camera.h"
#include "../../Graphics/SpriteFont.h"
#include "../../Graphics/SpriteBatch.h"
#include "../../Graphics/Texture.h"
#include "../../Utils/Text/Text.h"
#include "../../Windows/Application.h"
#include "../../Windows/OpenGL/OpenGLExtensions.h"
#include "../../Windows/OpenGL/glext.h"
#include "../../Windows/OpenGL/wglext.h"
#include <GameDev2D.h>


namespace GameDev2D
{
    Graphics::Graphics() :
        m_TexturedVertexData(nullptr),
        m_PolygonVertexData(nullptr),
        m_SpriteBatch(nullptr),
        m_ClearColor(Color::WhiteColor()),
        m_BoundShaderProgram(0),
        m_BoundTextureId(0),
        m_BoundFrameBufferId(0),
        m_BoundVertexArray(0),
        m_BoundDataBuffer(0),
        m_Stats(Graphics::Stats())
    {
        //Create the Camera object
		PushCamera(Camera());

        //Set the clear color
        SetClearColor(BACKGROUND_CLEAR_COLOR);
    }
    
    Graphics::~Graphics()
    {    
        if (m_TexturedVertexData != nullptr)
        {
            delete m_TexturedVertexData;
            m_TexturedVertexData = nullptr;
        }

		if (m_PolygonVertexData != nullptr)
		{
			delete m_PolygonVertexData;
			m_PolygonVertexData = nullptr;
		}

		if (m_SpriteBatch != nullptr)
		{
			delete m_SpriteBatch;
			m_SpriteBatch = nullptr;
		}
    }

    unsigned int Graphics::GetApiVersion()
    {
        float version;
        sscanf((char *)glGetString(GL_VERSION), "%f", &version);
        return (unsigned int)(100 * version);
    }

    unsigned int Graphics::GetShadingLanguageVersion()
    {
        float version;
        sscanf((char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "%f", &version);
        return (unsigned int)(100 * version);
    }
    
    void Graphics::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Graphics::DrawTexture(Texture* aTexture, Vector2 aPosition, Rotation aAngle, float aAlpha)
    {
        //Initialize the local variables used in this method
        Shader* shader = Services::GetResourceManager()->GetShader(PASSTHROUGH_TEXTURE_SHADER_KEY);
        Color color = Color::WhiteColor(aAlpha);
        Matrix transformation = Matrix::Make(aPosition, aAngle.GetRadians());

        //If the textured VertexData object hasn't been created yet, then, well... Create it!
        if (m_TexturedVertexData == nullptr)
        {
            //Initialize the Graphics's textured vertex VertexBufferDescriptor, it describes
            //how the individual 'vertices' will be stored in the DataBuffer
            VertexBufferDescriptor descriptor;
            descriptor.size = 4;
            descriptor.capacity = 4;
            descriptor.usage = BufferUsage_StaticDraw;
            descriptor.attributeIndex = { shader->GetAttributeLocation("in_vertices"), shader->GetAttributeLocation("in_uv") };
            descriptor.attributeSize = { 2, 2 };

            //Initialize the vertices
            std::vector<float> vertices;

            //Vertex 1
            vertices.push_back(0.0f);  //x
            vertices.push_back(0.0f);  //y
            vertices.push_back(0.0f);  //u
            vertices.push_back(0.0f);  //v

            //Vertex 2
            vertices.push_back(1.0f);  //x
            vertices.push_back(0.0f);  //y
            vertices.push_back(1.0f);  //u
            vertices.push_back(0.0f);  //v

            //Vertex 3
            vertices.push_back(0.0f);  //x
            vertices.push_back(1.0f);  //y
            vertices.push_back(0.0f);  //u
            vertices.push_back(1.0f);  //v

            //Vertex 4
            vertices.push_back(1.0f);  //x
            vertices.push_back(1.0f);  //y
            vertices.push_back(1.0f);  //u
            vertices.push_back(1.0f);  //v

            //Create the textured vertex DataBuffer
            m_TexturedVertexData = new VertexData();
            m_TexturedVertexData->CreateBuffer(descriptor, &vertices);
        }

        //Prepate the textured VertexBuffer for Drawing
        m_TexturedVertexData->PrepareForDraw();

        //Setup the shader uniforms
        shader->SetUniformVector2("uniform_anchor", Vector2(0.0f, 0.0f));
        shader->SetUniformVector2("uniform_size", Vector2((float)aTexture->GetWidth(), (float)aTexture->GetHeight()));
        shader->SetUniformFloat("uniform_color", color.r, color.g, color.b, color.a);
        shader->SetUniformInt("uniform_texture", 0);
        shader->SetUniformMatrix("uniform_model", transformation);
        shader->SetUniformMatrix("uniform_view", GetViewMatrix());
        shader->SetUniformMatrix("uniform_projection", GetProjectionMatrix());

        //Validate the shader, return if it fails
        if (shader->Validate() == false)
        {
            return;
        }

        //Bind the texture
        BindTexture(aTexture);

        //Enable blending if the texture has an alpha channel
        if (aTexture->GetPixelFormat().layout == PixelFormat::RGBA || color.a != 1.0f)
        {
            EnableBlending(BlendingMode());
        }

        //Draw the texture
        DrawArrays(RenderMode_TriangleStrip, 0, m_TexturedVertexData->GetVertexBuffer()->GetCount());

        //Disable blending, if we did in fact have it enabled
        if (aTexture->GetPixelFormat().layout == PixelFormat::RGBA || color.a != 1.0f)
        {
            DisableBlending();
        }

        //This unbind's the vertex array, so its not accidently modified
        m_TexturedVertexData->EndDraw();
    }

    void Graphics::DrawString(FontData* aFontData, const std::string& aText, Vector2 aPosition, Color aColor)
    {
        //Is there any text to render?
        if (aText.length() == 0)
        {
            return;
        }

        //If the SpriteBatch hasn't been created, create it
        if (m_SpriteBatch == nullptr)
        {
            m_SpriteBatch = new SpriteBatch();
        }

        //calculate the baseline and origin for the label
        unsigned int baseline = aFontData->lineHeight - aFontData->baseline;
        unsigned int numberOfLines = Text::NumberOfLines(aText);
        Vector2 origin(aPosition.x, aPosition.y + baseline + (aFontData->lineHeight * (numberOfLines - 1)));

        //Begin rendering to the SpriteBatch
        m_SpriteBatch->Begin();

        //Cycle through the characters in the text label
        for (unsigned int i = 0; i < aText.length(); i++)
        {
            //Did we reach a new line?
            if (aText.at(i) == '\n')
            {
                origin.x = aPosition.x;
                origin.y -= aFontData->lineHeight;
                continue;
            }

            //Get the character rect
			Rect characterRect = aFontData->glyphData[aText.at(i)].frame;

            //Calculate the character position based on the x and y bearing
            Vector2 charPosition = origin;
            charPosition.x += aFontData->glyphData[aText.at(i)].bearingX;
            charPosition.y += aFontData->glyphData[aText.at(i)].bearingY - characterRect.size.y;

            //Draw the
            m_SpriteBatch->Draw(aFontData->texture, charPosition, 0.0f, Vector2(1.0f, 1.0f), aColor, Vector2(0.0f, 0.0f), characterRect);

            //Increment the origin
            origin.x += aFontData->glyphData[aText.at(i)].advanceX;	
        }

        m_SpriteBatch->End();
    }

    void Graphics::DrawRectangle(Vector2 aPosition, Vector2 aSize, Rotation aAngle, Vector2 aAnchor, Color aColor, bool aIsFilled)
    {
        //Initialize the local variables used in this method
        Shader* shader = Services::GetResourceManager()->GetShader(PASSTHROUGH_POLYGON_SHADER_KEY);
        Matrix transformation = Matrix::Make(aPosition, aAngle.GetRadians());

        //If the textured VertexData object hasn't been created yet, then, well... Create it!
        if (m_PolygonVertexData == nullptr)
        {
            //Initialize the Graphics's textured vertex VertexBufferDescriptor, it describes
            //how the individual 'vertices' will be stored in the DataBuffer
            VertexBufferDescriptor descriptor;
            descriptor.size = 6;
            descriptor.capacity = 36; //Max vertices the circle can have
            descriptor.usage = BufferUsage_DynamicDraw;
            descriptor.attributeIndex = { shader->GetAttributeLocation("in_vertices"), shader->GetAttributeLocation("in_color") };
            descriptor.attributeSize = { 2, 4 };

            //Create the textured vertex DataBuffer
            m_PolygonVertexData = new VertexData();
            m_PolygonVertexData->CreateBuffer(descriptor);
        }

        //Clear the existing vertices and add the new vertices to the vertex buffer
        m_PolygonVertexData->GetVertexBuffer()->ClearVertices();
        m_PolygonVertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { 0.0f, 0.0f, aColor.r, aColor.g, aColor.b, aColor.a });
        m_PolygonVertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { aSize.x, 0.0f, aColor.r, aColor.g, aColor.b, aColor.a });
        m_PolygonVertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { aSize.x, aSize.y, aColor.r, aColor.g, aColor.b, aColor.a });
        m_PolygonVertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { 0.0f, aSize.y, aColor.r, aColor.g, aColor.b, aColor.a });

        //Prepate the textured VertexBuffer for Drawing
        m_PolygonVertexData->PrepareForDraw();

        //Setup the shader uniforms
        shader->SetUniformVector2("uniform_anchor", aAnchor);
        shader->SetUniformVector2("uniform_size", aSize);
        shader->SetUniformMatrix("uniform_model", transformation);
        shader->SetUniformMatrix("uniform_view", GetViewMatrix());
        shader->SetUniformMatrix("uniform_projection", GetProjectionMatrix());

        //Validate the shader, return if it fails
        if (shader->Validate() == false)
        {
            return;
        }

        //Enable blending if the texture has an alpha channel
        if (aColor.a != 1.0f)
        {
            EnableBlending(BlendingMode());
        }

        //Draw the texture
        DrawArrays(aIsFilled == true ? RenderMode_TriangleFan : RenderMode_LineLoop, 0, m_PolygonVertexData->GetVertexBuffer()->GetCount());

        //Disable blending, if we did in fact have it enabled
        if (aColor.a != 1.0f)
        {
            DisableBlending();
        }
    }

    void Graphics::DrawCircle(Vector2 aPosition, float aRadius, Vector2 aAnchor, Color aColor, bool aIsFilled)
    {
        //Initialize the local variables used in this method
        Shader* shader = Services::GetResourceManager()->GetShader(PASSTHROUGH_POLYGON_SHADER_KEY);
        Matrix transformation = Matrix::MakeTranslation(aPosition);

        //If the textured VertexData object hasn't been created yet, then, well... Create it!
        if (m_PolygonVertexData == nullptr)
        {
            //Initialize the Graphics's textured vertex VertexBufferDescriptor, it describes
            //how the individual 'vertices' will be stored in the DataBuffer
            VertexBufferDescriptor descriptor;
            descriptor.size = 6;
            descriptor.capacity = 36; //Max vertices the circle can have
            descriptor.usage = BufferUsage_DynamicDraw;
            descriptor.attributeIndex = { shader->GetAttributeLocation("in_vertices"), shader->GetAttributeLocation("in_color") };
            descriptor.attributeSize = { 2, 4 };

            //Create the textured vertex DataBuffer
            m_PolygonVertexData = new VertexData();
            m_PolygonVertexData->CreateBuffer(descriptor);
        }

        //Clear the existing vertices
        m_PolygonVertexData->GetVertexBuffer()->ClearVertices();

        //And add the new vertices to the vertex buffer for the circle
        for (float i = 0.0f; i < 359.99f; i += 10.0f)
        {
            float x = aRadius - (cosf(((float)M_PI * i / 180.0f)) * aRadius);
            float y = aRadius - (sinf(((float)M_PI * i / 180.0f)) * aRadius);
            m_PolygonVertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { x, y, aColor.r, aColor.g, aColor.b, aColor.a });
        }

        //Prepate the textured VertexBuffer for Drawing
        m_PolygonVertexData->PrepareForDraw();

        //Setup the shader uniforms
        shader->SetUniformVector2("uniform_anchor", aAnchor);
        shader->SetUniformVector2("uniform_size", Vector2(aRadius * 2.0f, aRadius * 2.0f));
        shader->SetUniformMatrix("uniform_model", transformation);
        shader->SetUniformMatrix("uniform_view", GetViewMatrix());
        shader->SetUniformMatrix("uniform_projection", GetProjectionMatrix());

        //Validate the shader, return if it fails
        if (shader->Validate() == false)
        {
            return;
        }

        //Enable blending if the texture has an alpha channel
        if (aColor.a != 1.0f)
        {
            EnableBlending(BlendingMode());
        }

        //Draw the texture
        DrawArrays(aIsFilled == true ? RenderMode_TriangleFan : RenderMode_LineLoop, 0, m_PolygonVertexData->GetVertexBuffer()->GetCount());

        //Disable blending, if we did in fact have it enabled
        if (aColor.a != 1.0f)
        {
            DisableBlending();
        }
    }

    void Graphics::DrawLine(Vector2 aStartPoint, Vector2 aEndPoint, Color aColor)
    {
        //Initialize the local variables used in this method
        Shader* shader = Services::GetResourceManager()->GetShader(PASSTHROUGH_POLYGON_SHADER_KEY);

        //If the textured VertexData object hasn't been created yet, then, well... Create it!
        if (m_PolygonVertexData == nullptr)
        {
            //Initialize the Graphics's textured vertex VertexBufferDescriptor, it describes
            //how the individual 'vertices' will be stored in the DataBuffer
            VertexBufferDescriptor descriptor;
            descriptor.size = 6;
            descriptor.capacity = 36; //Max vertices the circle can have
            descriptor.usage = BufferUsage_DynamicDraw;
            descriptor.attributeIndex = { shader->GetAttributeLocation("in_vertices"), shader->GetAttributeLocation("in_color") };
            descriptor.attributeSize = { 2, 4 };

            //Create the textured vertex DataBuffer
            m_PolygonVertexData = new VertexData();
            m_PolygonVertexData->CreateBuffer(descriptor);
        }

        //Clear the existing vertices
        m_PolygonVertexData->GetVertexBuffer()->ClearVertices();
        m_PolygonVertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { aStartPoint.x, aStartPoint.y, aColor.r, aColor.g, aColor.b, aColor.a });
        m_PolygonVertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { aEndPoint.x, aEndPoint.y, aColor.r, aColor.g, aColor.b, aColor.a });

        //Prepate the textured VertexBuffer for Drawing
        m_PolygonVertexData->PrepareForDraw();

        //Setup the shader uniforms
        shader->SetUniformVector2("uniform_anchor", Vector2(0.0f, 0.0f));
        shader->SetUniformVector2("uniform_size", aEndPoint - aStartPoint);
        shader->SetUniformMatrix("uniform_model", Matrix::Identity());
        shader->SetUniformMatrix("uniform_view", GetViewMatrix());
        shader->SetUniformMatrix("uniform_projection", GetProjectionMatrix());

        //Validate the shader, return if it fails
        if (shader->Validate() == false)
        {
            return;
        }

        //Enable blending if the texture has an alpha channel
        if (aColor.a != 1.0f)
        {
            EnableBlending(BlendingMode());
        }

        //Draw the texture
        DrawArrays(RenderMode_Lines, 0, m_PolygonVertexData->GetVertexBuffer()->GetCount());

        //Disable blending, if we did in fact have it enabled
        if (aColor.a != 1.0f)
        {
            DisableBlending();
        }
    }

    void Graphics::DrawArrays(RenderMode aRenderMode, int aFirst, int aCount)
    {
        unsigned int renderMode = GetOpenGLRenderMode(aRenderMode);
        glDrawArrays(renderMode, aFirst, aCount);
    }

    void Graphics::DrawElements(RenderMode aRenderMode, int aCount, GraphicType aType, const void* aIndices)
    {
        unsigned int renderMode = GetOpenGLRenderMode(aRenderMode);
        unsigned int type = GetOpenGLGraphicType(aType);
        glDrawElements(renderMode, aCount, type, aIndices);
    }

    Matrix Graphics::GetProjectionMatrix()
    {
        return GetActiveCamera()->GetProjectionMatrix();
    }

    Matrix Graphics::GetViewMatrix()
    {
        return GetActiveCamera()->GetViewMatrix();
    }

    Camera* Graphics::GetActiveCamera()
    {
        return &m_CameraStack.back();
    }

	void Graphics::PushCamera(const Camera& aCamera)
	{
		m_CameraStack.push_back(Camera(aCamera));

		//Set the OpenGL viewport
		SetViewport(GetActiveCamera()->GetViewport());
	}

	void Graphics::PopCamera()
	{
		m_CameraStack.pop_back();

		//Set the OpenGL viewport
		SetViewport(GetActiveCamera()->GetViewport());
	}
    
    void Graphics::EnableBlending(BlendingMode aBlendingMode)
    {
        //Determine the source and destination blending settings
        GLenum source = GetOpenGLBlendingFactor(aBlendingMode.source);
        GLenum destination = GetOpenGLBlendingFactor(aBlendingMode.destination);

        //Enable blending
        glBlendFunc(source, destination);
        glEnable(GL_BLEND);

        //Log the Graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Blending enabled");
    }

    void Graphics::DisableBlending()
    {
        //Disable blending
        glDisable(GL_BLEND);

        //Log the Graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Blending disabled");
    }

    void Graphics::SetClearColor(Color aClearColor)
    {
        //Set the OpenGL clear color
        m_ClearColor = aClearColor;
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);

        //Log the Graphic's event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Set clear color: (%f, %f, %f, %f)", m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
    }

    Color Graphics::GetClearColor()
    {
        return m_ClearColor;
    }

    void Graphics::SetViewport(const Viewport& aViewport)
    {
        //Set the OpenGL viewport size
        glViewport(0, 0, (int)aViewport.width, (int)aViewport.height);

        //Log the Graphic's event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Set viewport: (0, 0, %u, %u)", aViewport.width, aViewport.height);
    }

    void Graphics::PushScissorClip(float aX, float aY, float aWidth, float aHeight)
    {
        //If this is the first scissor clip, enable scissor clipping
        if (m_ScissorStack.size() == 0)
        {
            glEnable(GL_SCISSOR_TEST);
        }

        //Push the scissor clip onto the scissor stack
        m_ScissorStack.push_back(Rect(Vector2(aX, aY), Vector2(aWidth, aHeight)));

        //Reset the scissor clip
        ResetScissorClip();

        //Log the Graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Push scissor clip: (%f, %f, %f, %f)", aX, aY, aWidth, aHeight);
    }

    void Graphics::PopScissorClip()
    {
        //Pop back the scissor stack
        m_ScissorStack.pop_back();

        //Reset the scissor clip
        ResetScissorClip();

        //Log the Graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Pop scissor clip");
    }

    void Graphics::ResetScissorClip()
    {
        //Is there still clipping rects on the scissor stack?
        if (m_ScissorStack.size() > 0)
        {
            //Set the front scissor rect
            Vector2 position = m_ScissorStack.back().origin;
            Vector2 size = m_ScissorStack.back().size;
            glScissor((int)position.x, (int)position.y, (int)size.x, (int)size.y);
        }
        else
        {
            //Disable the scissor test
            glDisable(GL_SCISSOR_TEST);
        }
    }
    
    void Graphics::BindTexture(Texture* aTexture)
    {
        //Determine the Texture id to bind, if the pointer is null, it 
        //means we need to unbind (glBindTexture(0)) the active texture
        unsigned int id = 0;
        if (aTexture != nullptr)
        {
            id = aTexture->GetId();
        }

        //Is the texture already bound?
        if (id != m_BoundTextureId)
        {
            //Bind the texture id
            m_BoundTextureId = id;
            glBindTexture(GL_TEXTURE_2D, m_BoundTextureId);

            //Log the Graphics event
            Log::Message(Log::Verbosity_Graphics, "[Graphics] Bind texture: %u", m_BoundTextureId);
        }
    }


    unsigned int Graphics::GetBoundTextureId()
    {
        return m_BoundTextureId;
    }

    unsigned int Graphics::GenerateTexture()
    {
        //Generate a texture
        unsigned int id = 0;
        glGenTextures(1, &id);

        //Log the Graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Generate texture: %u", id);

        //Return the texture id
        return id;
    }

    void Graphics::DeleteTexture(Texture* aTexture)
    {
        //The Texture can't be null
        assert(aTexture != nullptr);

        //If the texture we are about to delete is bound, we need to unbind it
        if (aTexture->GetId() == GetBoundTextureId())
        {
            Services::GetGraphics()->BindTexture(nullptr);
        }

        //Get the pixel size, width and height
        unsigned int size = aTexture->GetPixelFormat().layout == PixelFormat::RGB ? 3 : 4;
        unsigned int width = aTexture->GetWidth();
        unsigned int height = aTexture->GetHeight();

        //Increment the texture memory stats
        m_Stats.textureMemory -= size * width * height;

        //Delete the texture
        unsigned int id = aTexture->GetId();
        glDeleteTextures(1, &id);

        //Log the Graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Delete texture: %u", aTexture->GetId());
    }

    void Graphics::SetTextureData(Texture* aTexture, const ImageData& aImageData)
    {
        //If this assert was hit, it means the textureData pointer is null
        assert(aTexture != nullptr);

        //Safety check that there's actually data to set
        if (aImageData.width > 0 && aImageData.height > 0)
        {
            //Get the pixel size, width and height
            unsigned int oldSize = aTexture->GetPixelFormat().layout == PixelFormat::RGB ? 3 : 4;
            unsigned int oldWidth = aTexture->GetWidth();
            unsigned int oldHeight = aTexture->GetHeight();

            //Increment the texture memory stats
            m_Stats.textureMemory -= oldSize * oldWidth * oldHeight;

            //Get the pixel size, width and height
            unsigned int size = aImageData.format.layout == PixelFormat::RGB ? 3 : 4;
            unsigned int width = aImageData.width;
            unsigned int height = aImageData.height;

            //Increment the texture memory stats
            m_Stats.textureMemory += size * width * height;

            //Bind the texture by its id
            BindTexture(aTexture);

            //Set the texture's filter settings
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            //Get the pixel format's layout (RGBA) and size
            unsigned int pixelLayout = GetOpenGLPixelLayout(aImageData.format.layout);
            unsigned int pixelSize = GetOpenGLPixelSize(aImageData.format.size);

            //Specify a 2D texture image, provideing the a pointer to the image data in memory
            glTexImage2D(GL_TEXTURE_2D, 0, pixelLayout, aImageData.width, aImageData.height, 0, pixelLayout, pixelSize, aImageData.data);
        }
    }

    void Graphics::SetTextureWrap(Texture* aTexture, Wrap aWrap)
    {
        //Bind the texture by its id
        BindTexture(aTexture);

        //Get the OpenGL wrap mode
        int wrapModeS = GetOpenGLWrapMode(aWrap.s);
        int wrapModeT = GetOpenGLWrapMode(aWrap.t);

        //Apply the wrap modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeT);
    }

    unsigned int Graphics::GetBoundFrameBufferId()
    {
        return m_BoundFrameBufferId;
    }

    void Graphics::BindFrameBuffer(unsigned int aFrameBuffer)
    {
        if (aFrameBuffer != m_BoundFrameBufferId)
        {
            m_BoundFrameBufferId = aFrameBuffer;
            glBindFramebuffer(GL_FRAMEBUFFER, m_BoundFrameBufferId);

            //Log the Graphics event
            Log::Message(Log::Verbosity_Graphics, "[Graphics] Bind framebuffer: %u", m_BoundFrameBufferId);
        }
    }

    void Graphics::GenerateFrameBuffer(unsigned int* aFrameBuffer)
    {
        //Generate the framebuffer
        glGenFramebuffers(1, aFrameBuffer);

        //Log the graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Generate framebuffer: %u", *aFrameBuffer);
    }

    void Graphics::DeleteFrameBuffer(unsigned int* aFrameBuffer)
    {
        //Delete the framebuffer
        glDeleteFramebuffers(1, aFrameBuffer);

        //Log the graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Delete framebuffer: %u", *aFrameBuffer);

        //Set the framebuffer to zero
        *aFrameBuffer = 0;
    }

    void Graphics::SetFrameBufferStorage(unsigned int aFrameBuffer, unsigned int aTextureId)
    {
        //Get the currently bound framebuffer
        unsigned int boundFrameBuffer = GetBoundFrameBufferId();

        //Bind the framebuffer
        BindFrameBuffer(aFrameBuffer);

        //If the texture id is bound, unbind it
        if (aTextureId == GetBoundTextureId())
        {
            Services::GetGraphics()->BindTexture(nullptr);
        }

        //Set the framebuffer storage to that of the texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aTextureId, 0);

        //Check for errors, if any are found; log an error
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Log::Error(false, Log::Verbosity_Graphics, "[Graphics] Failed to set framebuffer storage: %u", aFrameBuffer);
        }
        else
        {
            Log::Message(Log::Verbosity_Graphics, "[Graphics] Framebuffer %u storage set (texture %u)", aFrameBuffer, aTextureId);
        }

        //Bind the original framebuffer
        BindFrameBuffer(boundFrameBuffer);
    }

    void Graphics::CopyFrameBufferContents(unsigned int aFrameBuffer, PixelFormat aFormat, unsigned int aWidth, unsigned int aHeight, unsigned char* aBuffer)
    {
        //Bind the framebuffer
        BindFrameBuffer(aFrameBuffer);

        //Get the OpenGL texture format
        unsigned int layout = GetOpenGLPixelLayout(aFormat.layout);
        unsigned int size = GetOpenGLPixelSize(aFormat.size);

        //Read the data into the buffer
        glReadPixels(0, 0, aWidth, aHeight, layout, size, aBuffer);

        //Unbind the framebuffer
        BindFrameBuffer(0);
    }
    
    void Graphics::BindVertexArray(unsigned int aVertexArray)
    {
        //Is the vertex array already bound?
        if(aVertexArray != m_BoundVertexArray)
        {
            //Bind the vertex array
            m_BoundVertexArray = aVertexArray;
            glBindVertexArray(m_BoundVertexArray);

            //Log the Graphics event
            Log::Message(Log::Verbosity_Graphics, "[Graphics] Bind vertex array: %u", m_BoundVertexArray);
        }
    }
    
    unsigned int Graphics::GetBoundVertexArray()
    {
        return m_BoundVertexArray;
    }

    void Graphics::GenerateVertexArray(unsigned int* aVertexArray)
    {
        //Generate a new vertex array
        glGenVertexArrays(1, aVertexArray);

        //Log the Graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Generate vertex array: %u", *aVertexArray);
    }

    void Graphics::DeleteVertexArray(unsigned int* aVertexArray)
    {
        if (*aVertexArray != 0)
        {
            //Delete the vertex array
            glDeleteVertexArrays(1, aVertexArray);
            
            //Log the Graphics event
            Log::Message(Log::Verbosity_Graphics, "[Graphics] Delete vertex array: %u", *aVertexArray);

            //Zero the vertex array pointer value
            *aVertexArray = 0;
        }
    }

    void Graphics::BindDataBuffer(BufferType aBufferType, unsigned int aDataBuffer)
    {
        //Is the  buffer already bound?
        if (aDataBuffer != m_BoundDataBuffer)
        {
            //Get the OpenGL buffer type
            GLenum bufferType = GetOpenGLBufferType(aBufferType);

            //Bind the  buffer
            m_BoundDataBuffer = aDataBuffer;
            glBindBuffer(bufferType, aDataBuffer);

            //Log the Graphics event
            Log::Message(Log::Verbosity_Graphics, "[Graphics] Bind data buffer: %u", m_BoundDataBuffer);
        }
    }

    unsigned int Graphics::GetBoundDataBuffer()
    {
        return m_BoundDataBuffer;
    }

    void Graphics::GenerateDataBuffer(unsigned int* aDataBuffer)
    {
        //Generate the buffer
        glGenBuffers(1, aDataBuffer);

        //Log the Graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Generate buffer: %u", *aDataBuffer);
    }

    void Graphics::DeleteDataBuffer(unsigned int* aDataBuffer)
    {
        if (*aDataBuffer != 0)
        {
            //Delete the vertex buffer
            glDeleteBuffers(1, aDataBuffer);

            //Log the Graphics event
            Log::Message(Log::Verbosity_Graphics, "[Graphics] Delete buffer: %u", *aDataBuffer);

            //Zero the vertex buffer pointer value
            *aDataBuffer = 0;
        }
    }

    void Graphics::InitializeDataBuffer(unsigned int aDataBuffer, BufferType aBufferType, long long aSize, const void* aData, BufferUsage aUsage)
    {
        //Bind the vertex buffer
        BindDataBuffer(aBufferType, aDataBuffer);

        //Get the OpenGL usage constant
        GLenum usage = GetOpenGLBufferUsage(aUsage);

        //Get the OpenGL buffer type
        GLenum bufferType = GetOpenGLBufferType(aBufferType);

        //Set the vertex buffer data
        glBufferData(bufferType, (GLsizeiptr)aSize, aData, usage);
    }

    void Graphics::UpdateDataBuffer(unsigned int aDataBuffer, BufferType aBufferType, long long aOffset, long long aSize, const void* aData)
    {
        //Bind the data buffer
        BindDataBuffer(aBufferType, aDataBuffer);

        //Get the OpenGL buffer type
        GLenum bufferType = GetOpenGLBufferType(aBufferType);

        //Set the vertex buffer data
        glBufferSubData(bufferType, (GLintptr)aOffset, (GLsizeiptr)aSize, aData);
    }

    void Graphics::SetVertexAttributePointer(unsigned int aVertexBuffer, int aAttributeIndex, int aSize, int aStride, const void * aOffset)
    {
        //Bind the vertex buffer
        BindDataBuffer(BufferType_Vertex, aVertexBuffer);

        //Enable the vertex attribute array
        glEnableVertexAttribArray(aAttributeIndex);

        //Set the shader's attribute for the uv coordinates
        glVertexAttribPointer(aAttributeIndex, aSize, GL_FLOAT, GL_FALSE, aStride, aOffset);
    }

    unsigned int Graphics::CreateShaderProgram()
    {
        //Create the shader program
        unsigned int program = glCreateProgram();

        //Log the Graphics event
        Log::Message(Log::Verbosity_Graphics, "[Graphics] Create shader program: %u", program);

        //Return the shader program
        return program;
    }

    void Graphics::DeleteShaderProgram(unsigned int* aProgram)
    {
        if (*aProgram != 0)
        {
            //Delete the shader program
            glDeleteProgram(*aProgram);

            //Log the Graphics event
            Log::Message(Log::Verbosity_Graphics, "[Graphics] Delete shader program: %u", *aProgram);

            //Zero the shader program pointer value
            *aProgram = 0;
        }
    }

    unsigned int Graphics::CreateShader(ShaderType aShaderType)
    {
        //Get the OpenGL shader type
        GLenum shaderType = GetOpenGLShaderType(aShaderType);

        //Create the OpenGL shader for the shader type
        unsigned int shader = glCreateShader(shaderType);

        //Return the shader
        return shader;
    }

    void Graphics::DeleteShader(unsigned int* aShader)
    {
        if (*aShader != 0)
        {
            glDeleteShader(*aShader);
            *aShader = 0;
        }
    }

    bool Graphics::CompileShader(unsigned int aShader, const string& aSource)
    {
        //If this assert was hit, the shader hasn't been created yet
        assert(aShader != 0);

        //Get the GLSL version
        unsigned int version = GetShadingLanguageVersion();

        //Get the size of the version preprocessor string info so we know
        //how much memory to allocate for our sourceString
        int versionStringSize = sizeof("#version 123\n");

        //Allocate memory for the source string including the version preprocessor information
        char* shaderSource = (char*)malloc(aSource.length() + versionStringSize);

        //Prepend our vertex shader source string with the supported GLSL version so
        //the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
        sprintf(shaderSource, "#version %d\n%s", version, aSource.c_str());

        //Set the shader source
        glShaderSource(aShader, 1, &shaderSource, nullptr);

        //Compile the shader
        glCompileShader(aShader);

        //Get the compile status of the shader
        GLint status;
        glGetShaderiv(aShader, GL_COMPILE_STATUS, &status);

        //free the shader source array
        if (shaderSource != nullptr)
        {
            free(shaderSource);
            shaderSource = nullptr;
        }

        //Return the compile status of the shader
        return status != GL_FALSE;
    }

    void Graphics::AttachShader(unsigned int aProgram, unsigned int aShader)
    {
        glAttachShader(aProgram, aShader);
    }

    void Graphics::DetachShader(unsigned int aProgram, unsigned int aShader)
    {
        glDetachShader(aProgram, aShader);
    }

    bool Graphics::LinkShaderProgram(unsigned int aProgram)
    {
        //Link the shader program
        glLinkProgram(aProgram);

        //Get the link status of the program
        GLint status;
        glGetProgramiv(aProgram, GL_LINK_STATUS, &status);

        //Return the status
        return status != GL_FALSE;
    }

    bool Graphics::ValidateShaderProgram(unsigned int aProgram)
    {
        //Validate the shader program
        glValidateProgram(aProgram);

        //Check the status of the program
        int status;
        glGetProgramiv(aProgram, GL_VALIDATE_STATUS, &status);

        //Return the status
        return status != GL_FALSE;
    }

    void Graphics::UseShaderProgram(unsigned int aProgram)
    {
        if (aProgram != m_BoundShaderProgram)
        {
            m_BoundShaderProgram = aProgram;
            glUseProgram(m_BoundShaderProgram);
        }
    }

    int Graphics::GetUniformLocation(unsigned int aProgram, const string& aUniform)
    {
        return glGetUniformLocation(aProgram, aUniform.c_str());
    }

    int Graphics::GetAttributeLocation(unsigned int aProgram, const string& aAttribute)
    {
        return glGetAttribLocation(aProgram, aAttribute.c_str());
    }

    void Graphics::SetUniformFloat(unsigned int aProgram, int aLocation, float aValue)
    {
        UseShaderProgram(aProgram);
        glUniform1f(aLocation, aValue);
    }

    void Graphics::SetUniformFloat(unsigned int aProgram, int aLocation, float aValue1, float aValue2)
    {
        UseShaderProgram(aProgram);
        glUniform2f(aLocation, aValue1, aValue2);
    }

    void Graphics::SetUniformFloat(unsigned int aProgram, int aLocation, float aValue1, float aValue2, float aValue3)
    {
        UseShaderProgram(aProgram);
        glUniform3f(aLocation, aValue1, aValue2, aValue3);
    }

    void Graphics::SetUniformFloat(unsigned int aProgram, int aLocation, float aValue1, float aValue2, float aValue3, float aValue4)
    {
        UseShaderProgram(aProgram);
        glUniform4f(aLocation, aValue1, aValue2, aValue3, aValue4);
    }

    void Graphics::SetUniformInt(unsigned int aProgram, int aLocation, int aValue)
    {
        UseShaderProgram(aProgram);
        glUniform1i(aLocation, aValue);
    }

    void Graphics::SetUniformInt(unsigned int aProgram, int aLocation, int aValue1, int aValue2)
    {
        UseShaderProgram(aProgram);
        glUniform2i(aLocation, aValue1, aValue2);
    }

    void Graphics::SetUniformInt(unsigned int aProgram, int aLocation, int aValue1, int aValue2, int aValue3)
    {
        UseShaderProgram(aProgram);
        glUniform3i(aLocation, aValue1, aValue2, aValue3);
    }

    void Graphics::SetUniformInt(unsigned int aProgram, int aLocation, int aValue1, int aValue2, int aValue3, int aValue4)
    {
        UseShaderProgram(aProgram);
        glUniform4i(aLocation, aValue1, aValue2, aValue3, aValue4);
    }

    void Graphics::SetUniformVector2(unsigned int aProgram, int aLocation, const Vector2& aValue)
    {
        UseShaderProgram(aProgram);
        float data[2] = { aValue.x, aValue.y };
        glUniform2fv(aLocation, 1, &data[0]);
    }

    void Graphics::SetUniformMatrix(unsigned int aProgram, int aLocation, const Matrix& aValue)
    {
        UseShaderProgram(aProgram);
        glUniformMatrix4fv(aLocation, 1, 0, &aValue.m[0][0]);
    }

    unsigned int Graphics::GetOpenGLGraphicType(GraphicType aType)
    {
        unsigned int types[] = { GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE };
        return types[aType];
    }

    unsigned int Graphics::GetOpenGLPixelLayout(PixelFormat::PixelLayout aLayout)
    {
        unsigned int layouts[] = { GL_RGB, GL_RGBA };
        return layouts[aLayout];
    }

    unsigned int Graphics::GetOpenGLPixelSize(PixelFormat::PixelSize aSize)
    {
        unsigned int sizes[] = { GL_UNSIGNED_BYTE };
        return sizes[aSize];
    }

    unsigned int Graphics::GetOpenGLBlendingFactor(BlendingFactor aBlendingFactor)
    {
        GLenum blendingFactors[] = { GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA_SATURATE };
        return blendingFactors[aBlendingFactor];
    }

    int Graphics::GetOpenGLWrapMode(Wrap::WrapMode aWrapMode)
    {
        GLint wrapModes[] = { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_MIRRORED_REPEAT };
        return wrapModes[aWrapMode];
    }

    unsigned int Graphics::GetOpenGLRenderMode(RenderMode aRenderMode)
    {
        unsigned int renderModes[] = { GL_POINTS, GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON };
        return renderModes[aRenderMode];
    }

    unsigned int Graphics::GetOpenGLBufferUsage(BufferUsage aBufferUsage)
    {
        unsigned int bufferUsages[] = { GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY };
        return bufferUsages[aBufferUsage];
    }

    unsigned int Graphics::GetOpenGLBufferType(BufferType aBufferType)
    {
        unsigned int bufferTypes[] = { GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER };
        return bufferTypes[aBufferType];
    }

    unsigned int Graphics::GetOpenGLShaderType(ShaderType aShaderType)
    {
        GLenum shaderTypes[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER };
        return shaderTypes[aShaderType];
    }

    unsigned long long Graphics::GetAllocatedTextureMemory()
    {
        return m_Stats.textureMemory;
    }
}
