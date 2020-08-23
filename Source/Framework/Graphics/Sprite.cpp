#include "Sprite.h"
#include "Shader.h"
#include "../Core/Drawable.h"
#include "../Events/TextureResourceEvent.h"
#include "../Services/Services.h"
#include "../Services/Graphics/Graphics.h"
#include "../Windows/Application.h"
#include <GameDev2D.h>
#include <vector>


namespace GameDev2D
{
    Sprite::Sprite(Texture* aTexture) : Drawable(),
        m_Texture(aTexture),
        m_VertexData(nullptr),
        m_Frame(Rect()),
        m_Wrap(Wrap())
    {
        //Initialize the Shader
        m_Shader = Services::GetResourceManager()->GetShader(PASSTHROUGH_TEXTURE_SHADER_KEY);

        //Initialize the Sprite's vertex DataBufferDescriptor, it describes
        //how the individual 'elements' will be stored in the DataBuffer
        VertexBufferDescriptor descriptor;
        descriptor.size = SPRITE_VERTEX_SIZE;
        descriptor.capacity = SPRITE_VERTEX_COUNT;
        descriptor.usage = BufferUsage_DynamicDraw;
        descriptor.attributeIndex = { m_Shader->GetAttributeLocation("in_vertices"), m_Shader->GetAttributeLocation("in_uv") };
        descriptor.attributeSize = { 2, 2 };

        //Create the vertex DataBuffer
        m_VertexData = new VertexData();
        m_VertexData->CreateBuffer(descriptor);

        //Set the Texture's frame
        SetFrame(Rect(Vector2(0.0f, 0.0f), Vector2((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight())));

        //Set the wrap mode
        SetWrap(Wrap(Wrap::ClampToEdge, Wrap::ClampToEdge));

		//Listener for the texture resource event, in case the texture the sprite is drawing is accidently unloaded, IT HAS HAPPENED!
		Services::GetResourceManager()->AddEventListener(this, TEXTURE_RESOURCE_UNLOADED);
    }
    
	Sprite::Sprite(const std::string& aFilename) : Drawable(),
		m_Texture(nullptr),
		m_VertexData(nullptr),
		m_Frame(Rect()),
		m_Wrap(Wrap())
	{
		//Initialize the Shader
		m_Shader = Services::GetResourceManager()->GetShader(PASSTHROUGH_TEXTURE_SHADER_KEY);

		//Initialize the Sprite's vertex DataBufferDescriptor, it describes
		//how the individual 'elements' will be stored in the DataBuffer
		VertexBufferDescriptor descriptor;
		descriptor.size = SPRITE_VERTEX_SIZE;
		descriptor.capacity = SPRITE_VERTEX_COUNT;
		descriptor.usage = BufferUsage_DynamicDraw;
		descriptor.attributeIndex = { m_Shader->GetAttributeLocation("in_vertices"), m_Shader->GetAttributeLocation("in_uv") };
		descriptor.attributeSize = { 2, 2 };

		//Create the vertex DataBuffer
		m_VertexData = new VertexData();
		m_VertexData->CreateBuffer(descriptor);

		//Initialize the texture
		SetTexture(Services::GetResourceManager()->GetTexture(aFilename));

		//Set the wrap mode
		SetWrap(Wrap(Wrap::ClampToEdge, Wrap::ClampToEdge));

		//Listener for the texture resource event, in case the texture the sprite is drawing is accidently unloaded, IT HAS HAPPENED!
		Services::GetResourceManager()->AddEventListener(this, TEXTURE_RESOURCE_UNLOADED);
	}

    Sprite::~Sprite()
    {
        if (m_VertexData != nullptr)
        {
            delete m_VertexData;
            m_VertexData = nullptr;
        }

        m_Texture = nullptr;

		Services::GetResourceManager()->RemoveEventListener(this, TEXTURE_RESOURCE_UNLOADED);
    }
    
    void Sprite::Draw()
    {
        //Is there a Texture to Draw?
        if (m_Texture != nullptr)
        {
            //Cache the Graphics pointer
            Graphics* graphics = Services::GetGraphics();

            //Bind the vertex array object
            m_VertexData->PrepareForDraw();
 
            //Setup the shader uniforms
            m_Shader->SetUniformVector2("uniform_anchor", GetAnchor());
            m_Shader->SetUniformVector2("uniform_size", Vector2(GetWidth(), GetHeight()));
            m_Shader->SetUniformFloat("uniform_color", m_Color.r, m_Color.g, m_Color.b, m_Color.a);
            m_Shader->SetUniformInt("uniform_texture", 0);
            m_Shader->SetUniformMatrix("uniform_model", GetTransformMatrix());
            m_Shader->SetUniformMatrix("uniform_view", graphics->GetViewMatrix());
            m_Shader->SetUniformMatrix("uniform_projection", graphics->GetProjectionMatrix());

            //Validate the shader, return if it fails
            if (m_Shader->Validate() == false)
            {
                return;
            }

            //Bind the texture
            graphics->BindTexture(m_Texture);

            //Enable blending if the texture has an alpha channel
            if (m_Texture->GetPixelFormat().layout == PixelFormat::RGBA || m_Color.a != 1.0f)
            {
                graphics->EnableBlending(GetBlendingMode());
            }

            //Draw the texture
            graphics->DrawArrays(RenderMode_TriangleStrip, 0, m_VertexData->GetVertexBuffer()->GetCount());

            //Disable blending, if we did in fact have it enabled
            if (m_Texture->GetPixelFormat().layout == PixelFormat::RGBA || m_Color.a != 1.0f)
            {
                graphics->DisableBlending();
            }

            //This unbind's the vertex array, so its not accidently modified
            m_VertexData->EndDraw();

#if DEBUG && DEBUG_DRAW_SPRITE_RECT
            //Draw the Debug Rectangle
            Vector2 size = Vector2(GetWidth() * GetScale().x, GetHeight() * GetScale().y);
            graphics->DrawRectangle(GetTransformMatrix().GetTranslation(), size, GetTransformMatrix().GetRotation(), GetAnchor(), Color::BlueColor(), false);

            

            //Draw the origin lines
            Vector2 axisSize = Vector2(16.0f, 16.0f);
            Vector2 offset1 = axisSize * GetDirection();
            Vector2 offset2 = axisSize * Rotation::Degrees(GetTransformMatrix().GetRotation().GetDegrees() + 90.0f).GetDirection();
			Vector2 origin = GetTransformMatrix().GetTranslation();

            graphics->DrawLine(origin, origin + offset1, Color::BlueColor());
            graphics->DrawLine(origin, origin + offset2, Color::GreenColor());
#endif

#if DEBUG && DEBUG_DRAW_SPRITE_AABB
			float left = GetLeftEdge();
			float right = GetRightEdge();
			float top = GetTopEdge();
			float bottom = GetBottomEdge();
			graphics->DrawLine(Vector2(left, top), Vector2(right, top), Color::RedColor());
			graphics->DrawLine(Vector2(left, bottom), Vector2(right, bottom), Color::RedColor());
			graphics->DrawLine(Vector2(left, top), Vector2(left, bottom), Color::RedColor());
			graphics->DrawLine(Vector2(right, top), Vector2(right, bottom), Color::RedColor());
#endif
        }
    }

    float Sprite::GetWidth()
    {
        return m_Frame.size.x;
    }

	float Sprite::GetHeight()
	{
		return m_Frame.size.y;
	}

    void Sprite::SetWrap(Wrap::WrapMode aWrapModeS, Wrap::WrapMode aWrapModeT)
    {
        //Set the wrap settings
        m_Wrap.s = aWrapModeS;
        m_Wrap.t = aWrapModeT;

        //Apply the texture's wrap
        if (m_Texture != nullptr)
        {
            Services::GetGraphics()->SetTextureWrap(m_Texture, m_Wrap);
        }
    }

    void Sprite::SetWrap(Wrap aWrap)
    {
        SetWrap(aWrap.s, aWrap.t);
    }

    Wrap Sprite::GetWrap()
    {
        return m_Wrap;
    }

    PixelFormat Sprite::GetPixelFormat()
    {
        if (m_Texture != nullptr)
        {
            return m_Texture->GetPixelFormat();
        }
        return PixelFormat();
    }

	void Sprite::SetFrame(Rect aFrame)
	{
		//Set the source frame
		m_Frame.origin = aFrame.origin;
		m_Frame.size = aFrame.size;

		if (m_Texture != nullptr && m_VertexData != nullptr)
		{
			//Build the UV Coordinates
			float u1 = (float)m_Frame.origin.x / (float)m_Texture->GetWidth();
			float u2 = (float)(m_Frame.origin.x + m_Frame.size.x) / (float)m_Texture->GetWidth();
			float v1 = 1.0f - (((float)m_Frame.origin.y + m_Frame.size.y) / (float)m_Texture->GetHeight());
			float v2 = 1.0f - (m_Frame.origin.y / (float)m_Texture->GetHeight());

			//Clear the vertices from the VertexBuffer 
			m_VertexData->GetVertexBuffer()->ClearVertices();

			//Add the new vertices to the VertexBuffer
			m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { 0.0f, 0.0f, u1, v1 });
			m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { 1.0f, 0.0f, u2, v1 });
			m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { 0.0f, 1.0f, u1, v2 });
			m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>() = { 1.0f, 1.0f, u2, v2 });
		}
	}

    Rect Sprite::GetFrame()
    {
        return m_Frame;
    }

    void Sprite::SetTexture(Texture* aTexture)
    {
		//Set the Texture's
        m_Texture = aTexture;

		//Set the Texture's frame
		SetFrame(Rect(Vector2(0.0f, 0.0f), Vector2((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight())));
    }
        
    Texture* Sprite::GetTexture() const
    {
        return m_Texture;
    }

	void Sprite::HandleEvent(Event* aEvent)
	{
		if (aEvent->GetEventCode() == TEXTURE_RESOURCE_UNLOADED)
		{
			TextureResourceEvent* textureResourceEvent = (TextureResourceEvent*)aEvent;
			if (textureResourceEvent->GetTexture() == m_Texture)
			{
				//The texture the Sprite is using was just unloaded, replace it with the default texture
				SetTexture(Services::GetResourceManager()->GetDefaultTexture());
			}
		}
	}
}