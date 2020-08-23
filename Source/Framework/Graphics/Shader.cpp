#include "Shader.h"
#include "../Debug/Log.h"
#include "../Services/Services.h"
#include <assert.h>


namespace GameDev2D
{
    Shader::Shader(const ShaderSource& aShaderSource) : 
        m_Program(0),
        m_VertexShader(0),
        m_FragmentShader(0),
        m_GeometryShader(0),
        m_Key("")
    {
        //Create the shader program
        m_Program = Services::GetGraphics()->CreateShaderProgram();

        //If this assert is hit, it means the vertex shader that was passed in has no content
        assert(aShaderSource.vertex.length() > 0);

        //Log the vertex shader
        Log::Message(Log::Verbosity_Shaders, "[Shader] Vertex shader\n%s", aShaderSource.vertex);

        //Create the vertex shader
        m_VertexShader = Services::GetGraphics()->CreateShader(VertexShader);

        //Compile the vertex shader 
        bool success = Services::GetGraphics()->CompileShader(m_VertexShader, aShaderSource.vertex);

        //If this assert is hit, it means the vertex shader did not compile
        assert(success == true);

        //Attach the vertex shader to the shader program
        Services::GetGraphics()->AttachShader(m_Program, m_VertexShader);

        //If this assert is hit, that means the fragment shader that was passed in has no content
        assert(aShaderSource.fragment.length() > 0);

        //Log the fragment shader
        Log::Message(Log::Verbosity_Shaders, "[Shader] Fragment shader\n%s", aShaderSource.fragment);

        //Create the fragment shader
        m_FragmentShader = Services::GetGraphics()->CreateShader(FragmentShader);

        //Compile the fragment shader 
        success = Services::GetGraphics()->CompileShader(m_FragmentShader, aShaderSource.fragment);

        //If this assert is hit, it means the fragment shader did not compile
        assert(success == true);

        //Attach the fragment shader to the shader program
        Services::GetGraphics()->AttachShader(m_Program, m_FragmentShader);

        //Is there a geometry shader?
        if (aShaderSource.geometry.length() > 0)
        {
            //Log the geometry shader
            Log::Message(Log::Verbosity_Shaders, "[Shader] Geometry shader\n%s", aShaderSource.geometry);

            //Create the geometry shader
            m_GeometryShader = Services::GetGraphics()->CreateShader(GeometryShader);

            //Compile the geometry shader 
            success = Services::GetGraphics()->CompileShader(m_GeometryShader, aShaderSource.geometry);

            //If this assert is hit, it means the fragment shader did not compile
            assert(success == true);

            //Attach the fragment shader to the shader program
            Services::GetGraphics()->AttachShader(m_Program, m_GeometryShader);
        }


        //Link the program
        success = Services::GetGraphics()->LinkShaderProgram(m_Program);

        //Was linking successful?
        if (success == true)
        {
            //Always detach shaders after a successful link
            Services::GetGraphics()->DetachShader(m_Program, m_VertexShader);
            Services::GetGraphics()->DetachShader(m_Program, m_FragmentShader);
        }
        else
        {
            //Delete the shader program
            Services::GetGraphics()->DeleteShaderProgram(&m_Program);

            //Delete the shaders
            Services::GetGraphics()->DeleteShader(&m_VertexShader);
            Services::GetGraphics()->DeleteShader(&m_FragmentShader);

            //Log the error
            Log::Error(true, Log::Verbosity_Shaders, "[Shader] Failed to link shader program\n\nVertex source: %s\n\n\nFragment source: %s\n", aShaderSource.vertex.c_str(), aShaderSource.fragment.c_str());
        }
    }

    Shader::~Shader()
    {
        //Delete the shader program
        Services::GetGraphics()->DeleteShaderProgram(&m_Program);

        //Delete the shaders
        Services::GetGraphics()->DeleteShader(&m_VertexShader);
        Services::GetGraphics()->DeleteShader(&m_FragmentShader);
    }

    void Shader::AddAttribute(const string& aAttribute)
    {
        int location = Services::GetGraphics()->GetAttributeLocation(m_Program, aAttribute);
        m_AttributeLocations[string(aAttribute)] = location;
    }

    int Shader::GetAttributeLocation(const string& aAttribute)
    {
        int index = m_AttributeLocations[string(aAttribute)];
        return index;
    }

    void Shader::AddUniform(const string& aUniform)
    {
        int location = Services::GetGraphics()->GetUniformLocation(m_Program, aUniform);
        m_UniformLocations[string(aUniform)] = location;
    }

    int Shader::GetUniformLocation(const string& aUniform)
    {
        int index = m_UniformLocations[string(aUniform)];
        return index;
    }

    void Shader::SetUniformFloat(const string& aUniform, float aValue)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformFloat(m_Program, location, aValue);
    }

    void Shader::SetUniformFloat(const string& aUniform, float aValue1, float aValue2)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformFloat(m_Program, location, aValue1, aValue2);
    }

    void Shader::SetUniformFloat(const string& aUniform, float aValue1, float aValue2, float aValue3)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformFloat(m_Program, location, aValue1, aValue2, aValue3);
    }

    void Shader::SetUniformFloat(const string& aUniform, float aValue1, float aValue2, float aValue3, float aValue4)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformFloat(m_Program, location, aValue1, aValue2, aValue3, aValue4);
    }

    void Shader::SetUniformInt(const string& aUniform, int aValue)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformInt(m_Program, location, aValue);
    }

    void Shader::SetUniformInt(const string& aUniform, int aValue1, int aValue2)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformInt(m_Program, location, aValue1, aValue2);
    }

    void Shader::SetUniformInt(const string& aUniform, int aValue1, int aValue2, int aValue3)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformInt(m_Program, location, aValue1, aValue2, aValue3);
    }

    void Shader::SetUniformInt(const string& aUniform, int aValue1, int aValue2, int aValue3, int aValue4)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformInt(m_Program, location, aValue1, aValue2, aValue3, aValue4);
    }

    void Shader::SetUniformVector2(const string& aUniform, const Vector2& aValue)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformVector2(m_Program, location, aValue);
    }

    void Shader::SetUniformMatrix(const string& aUniform, const Matrix& aValue)
    {
        int location = GetUniformLocation(aUniform);
        Services::GetGraphics()->SetUniformMatrix(m_Program, location, aValue);
    }

    bool Shader::Validate()
    {
        return Services::GetGraphics()->ValidateShaderProgram(m_Program);
    }

    void Shader::SetKey(const string& aKey)
    {
        m_Key = string(aKey);
    }

    string Shader::GetKey()
    {
        return m_Key;
    }
}
