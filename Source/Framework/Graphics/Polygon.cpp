#include "Polygon.h"
#include "../Services/Services.h"
#include "../Services/Graphics/Graphics.h"


namespace GameDev2D
{ 
    Polygon::Polygon() : Drawable(),
        m_VertexData(nullptr),
        m_RenderMode(RenderMode_Points),
        m_Size(0.0f, 0.0f),
        m_EnableBlending(false)
    {
        //Initialize the Shader
        m_Shader = Services::GetResourceManager()->GetShader(PASSTHROUGH_POLYGON_SHADER_KEY);

        //Initialize the Polygon's VertexBuffer VertexDescriptor, it describes
        //how the individual vertices will be stored in the VertexBuffer
        VertexBufferDescriptor descriptor;
        descriptor.size = Vertex::vertexSize;
        descriptor.capacity = POLYGON_MAX_VERTICES;
        descriptor.usage = BufferUsage_DynamicDraw;
        descriptor.attributeIndex = { m_Shader->GetAttributeLocation("in_vertices"), m_Shader->GetAttributeLocation("in_color") };
        descriptor.attributeSize = { 2, 4 };

        //Create the vertex DataBuffer
        m_VertexData = new VertexData();
        m_VertexData->CreateBuffer(descriptor);
    }
    
    Polygon::~Polygon()
    {
        if (m_VertexData != nullptr)
        {
            delete m_VertexData;
            m_VertexData;
        }
    }
    
    void Polygon::Draw()
    {
        //Get the number of vertices
        const unsigned int numberOfVertices = GetNumberOfVertices();

        //Safety check the vertices
        if (numberOfVertices == 0)
        {
            return;
        }

        //Cache the graphics service
        Graphics* graphics = Services::GetGraphics();

        //Bind the vertex array object
        m_VertexData->PrepareForDraw();

        //Setup the shader uniforms
        m_Shader->SetUniformVector2("uniform_anchor", GetAnchor());
        m_Shader->SetUniformVector2("uniform_size", Vector2(GetWidth(), GetHeight()));
        m_Shader->SetUniformMatrix("uniform_model", GetTransformMatrix());
        m_Shader->SetUniformMatrix("uniform_view", graphics->GetViewMatrix());
        m_Shader->SetUniformMatrix("uniform_projection", graphics->GetProjectionMatrix());

        //Validate the shader, return if it fails
        if (m_Shader->Validate() == false)
        {
            return;
        }

        //Enable blending if the texture has an alpha channel
        if (m_EnableBlending == true)
        {
            graphics->EnableBlending(GetBlendingMode());
        }

        //Draw the texture
        graphics->DrawArrays(m_RenderMode, 0, m_VertexData->GetVertexBuffer()->GetCount());

        //Disable blending, if we did in fact have it enabled
        if (m_EnableBlending == true)
        {
            graphics->DisableBlending();
        }
    }

    float Polygon::GetWidth()
    {
        return m_Size.x;
    }

    float Polygon::GetHeight()
    {
        return m_Size.y;
    }

	void Polygon::MakeCircle(float aRadius, bool aIsFilled, unsigned int aLineSegments)
	{
		//The radius can't be negative OR zero
		assert(aRadius > 0.0f);

		//Set the render mode
		SetRenderMode(aIsFilled == true ? RenderMode_TriangleFan : RenderMode_LineLoop);

		//Clear any existing vertices
		ClearVertices();

		//Calculate the rotation amount
		float rotationAmount = (360.0f / (float)aLineSegments);

		//Set the vertices
		Vector2 vertex;
		for (float i = 0; i < 359.99f; i += rotationAmount)
		{
			vertex.x = aRadius - (cosf(((float)M_PI * i / 180.0f)) * aRadius);
			vertex.y = aRadius - (sinf(((float)M_PI * i / 180.0f)) * aRadius);
			AddVertex(vertex);
		}
	}

	void Polygon::MakeRectangle(float aWidth, float aHeight, bool aIsFilled)
	{
		//The width and height can't be negative OR zero
		assert(aWidth > 0.0f && aHeight > 0.0f);

		//Set the render mode
		SetRenderMode(aIsFilled == true ? RenderMode_TriangleFan : RenderMode_LineLoop);

		//Clear any existing vertices
		ClearVertices();

		//Add the vertices
		AddVertex(Vector2(0.0f, 0.0f));
		AddVertex(Vector2(aWidth, 0.0f));
		AddVertex(Vector2(aWidth, aHeight));
		AddVertex(Vector2(0.0f, aHeight));
	}

    void Polygon::AddVertex(float aX, float aY)
    {
        AddVertex(Vector2(aX, aY));
    }

    void Polygon::AddVertex(Vector2 aVertex)
    {
        //Add the vertex to the vertices vector
        m_Vertices.push_back(Vertex(aVertex, GetColor()));

        //Add the vertex to the VertexBuffer
        float* data = m_Vertices.back().data;
        m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>(data, data + Vertex::vertexSize));

        //Do we need to enable blending
        if (GetColor().a != 1.0f)
        {
            m_EnableBlending = true;
        }

        //Calculate the polygon's size
        CalculateSize();
    }

    unsigned int Polygon::GetNumberOfVertices()
    {
        return m_Vertices.size();
    }

    void Polygon::ClearVertices()
    {
        //Clear the vertices vector
        m_Vertices.clear();

        //Clear the vertex buffer's vertices
        m_VertexData->GetVertexBuffer()->ClearVertices();

        //Set blending to false
        m_EnableBlending = false;
    }

    void Polygon::SetRenderMode(RenderMode aRenderMode)
    {
        m_RenderMode = aRenderMode;
    }

	RenderMode Polygon::GetRenderMode()
	{
		return m_RenderMode;
	}

	void Polygon::SetColor(Color aColor)
	{
		//Call the Drawable's SetColor method
		Drawable::SetColor(aColor);

		//Clear the vertices
		m_VertexData->GetVertexBuffer()->ClearVertices();

		//Loop through and set the vertices color
		for (unsigned int i = 0; i < m_Vertices.size(); i++)
		{
			m_Vertices.at(i).data[2] = GetColor().r;
			m_Vertices.at(i).data[3] = GetColor().g;
			m_Vertices.at(i).data[4] = GetColor().b;
			m_Vertices.at(i).data[5] = GetColor().a;

			//Re-add the vertex to the VertexBuffer
			float* data = m_Vertices.at(i).data;
			m_VertexData->GetVertexBuffer()->AddVertex(std::vector<float>(data, data + Vertex::vertexSize));
		}

		//Do we need to enable blending
		if (GetColor().a != 1.0f)
		{
			m_EnableBlending = true;
		}
	}

    void Polygon::CalculateSize()
    {
        //Local variables used below
        float left = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
        float top = 0.0f;

        //Cycle through and set the left, right, bottom and top vars
        for (unsigned int i = 0; i < m_Vertices.size(); i++)
        {
            left = fminf(left, m_Vertices.at(i).GetPosition().x);
            right = fmaxf(right, m_Vertices.at(i).GetPosition().x);
            bottom = fminf(bottom, m_Vertices.at(i).GetPosition().y);
            top = fmaxf(top, m_Vertices.at(i).GetPosition().y);
        }

        //Set the width and height
        m_Size.x = right - left;
        m_Size.y = top - bottom;
    }
}