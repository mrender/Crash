#ifndef __GameDev2D__Shader__
#define __GameDev2D__Shader__

#include "../Math/Vector2.h"
#include "../Math/Matrix.h"
#include <map>
#include <string>
#include <vector>


namespace GameDev2D
{
    //Struct to manager the shader sources, attributes and uniforms
    struct ShaderInfo
    {
        ShaderInfo(const std::string& vertex, const std::string& fragment, const std::vector<std::string>& attributes, const std::vector<std::string>& uniforms) :
            vertex(vertex),
            fragment(fragment),
            geometry(""),
            attributes(attributes),
            uniforms(uniforms)
        {

        }

        ShaderInfo(const std::string& vertex, const std::string& fragment, const std::string& geometry, const std::vector<std::string>& attributes, const std::vector<std::string>& uniforms) :
            vertex(vertex),
            fragment(fragment),
            geometry(geometry),
            attributes(attributes),
            uniforms(uniforms)
        {

        }

        const std::string vertex;
        const std::string fragment;
        const std::string geometry;
        const std::vector<std::string> attributes;
        const std::vector<std::string> uniforms;
    };

    struct ShaderSource
    {
        ShaderSource(const std::string& vertex, const std::string& fragment) :
            vertex(vertex),
            fragment(fragment),
            geometry("")
        {
        }

        ShaderSource(const std::string& vertex, const std::string& fragment, const std::string& geometry) :
            vertex(vertex),
            fragment(fragment),
            geometry(geometry)
        {
        }

        const std::string vertex;
        const std::string fragment;
        const std::string geometry;
    };


    //A Shader is part of the customizable rendering pipeline
    class Shader
    {
    public:
        //Adds an attribute
        void AddAttribute(const std::string& attribute);

        //Returns an attribute index for a attribute key
        int GetAttributeLocation(const std::string& attribute);

        //Adds an attribute
        void AddUniform(const std::string& uniform);

        //Returns an attribute index for a uniform key
        int GetUniformLocation(const std::string& uniform);

        //Sets float value(s) for the uniform key
        void SetUniformFloat(const std::string& uniform, float value);
        void SetUniformFloat(const std::string& uniform, float value1, float value2);
        void SetUniformFloat(const std::string& uniform, float value1, float value2, float value3);
        void SetUniformFloat(const std::string& uniform, float value1, float value2, float value3, float value4);

        //Sets int value(s) for the uniform key
        void SetUniformInt(const std::string& uniform, int value);
        void SetUniformInt(const std::string& uniform, int value1, int value2);
        void SetUniformInt(const std::string& uniform, int value1, int value2, int value3);
        void SetUniformInt(const std::string& uniform, int value1, int value2, int value3, int value4);

        //Sets a Vector2 for the uniform key
        void SetUniformVector2(const std::string& uniform, const Vector2& value);

        //Sets a Matrix for the uniform key
        void SetUniformMatrix(const std::string& uniform, const Matrix& value);

        //Validate the Shader, returns true is the validation was successful
        bool Validate();

        //Set a key to identify this shader from other shader
        void SetKey(const std::string& key);

        //Returns the key for the shader
        std::string GetKey();

    protected:
        //Created from the ResourceManager with the supplied ShaderSourcce
        Shader(const ShaderSource& shaderSource);
        ~Shader();

        //The ShaderManager needs to access the protected methods
        friend class ResourceManager;

    private:
        //Member variables
        std::map<std::string, int> m_UniformLocations;
        std::map<std::string, int> m_AttributeLocations;
        unsigned int m_Program;
        unsigned int m_VertexShader;
        unsigned int m_FragmentShader;
        unsigned int m_GeometryShader;
        std::string m_Key;
    };
}

#endif