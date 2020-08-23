#ifndef __GameDev2D__VertexData__
#define __GameDev2D__VertexData__

#include "GraphicTypes.h"
#include "../Debug/Log.h"
#include <assert.h>
#include <string>
#include <vector>



namespace GameDev2D
{
    //Forward declarations
    class VertexBuffer;
    class IndexBuffer;

    //The VertexData is really an OpenGL vertex array object in disguise, useful for managing VBOs
    class VertexData
    {
    public:
        VertexData();
        ~VertexData();

        //Create a VertexBuffer for the given descriptor, data can be provided or it can be set or modified later
        VertexBuffer* CreateBuffer(const VertexBufferDescriptor& descriptor, std::vector<float>* data = nullptr);

        //Create a IndexBuffer for the given descriptor, data can be provided or it can be set or modified later
        IndexBuffer* CreateBuffer(const IndexBufferDescriptor& descriptor, std::vector<unsigned short>* data = nullptr);

        //Returns the VertexBuffer, can be null if one wasn't created
        VertexBuffer* GetVertexBuffer();

        //Return the IndexBuffer, can be null if one wasn't created
        IndexBuffer* GetIndexBuffer();

        //Called in a Draw method right before you want to draw
        void PrepareForDraw();

        //Called in a Draw method right after you are done drawing
        void EndDraw();

    private:
        //Member variables
        unsigned int m_VertexArray;
        VertexBuffer* m_VertexBuffer;
        IndexBuffer* m_IndexBuffer;
    };

    //Base Buffer class
    class Buffer
    {
    public:
        Buffer(BufferType type, BufferUsage usage, unsigned int size, unsigned int capacity);
        virtual ~Buffer();

        virtual void UpdateBuffer() = 0;

        BufferType GetType();
        BufferUsage GetUsage();
        unsigned int GetSize();
        unsigned int GetCount();
        unsigned int GetCapacity();

    protected:
        //Member variables
        BufferType m_Type;
        BufferUsage m_Usage;
        unsigned int m_Size;
        unsigned int m_Capacity;
        unsigned int m_Count;
        unsigned int m_BufferId;
        bool m_IsDirty;
    };

    //Vertex Buffer 
    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(const VertexBufferDescriptor& aDescriptor, std::vector<float>* data);
        ~VertexBuffer();

        void UpdateBuffer();

        void AddVertex(const std::vector<float>& vertex);
        void ClearVertices();

    private:
        void UpdateAttributes();

        float* m_Buffer;
        std::vector<int> m_AttributeIndex;
        std::vector<unsigned int> m_AttributeSize;
    };

    //Index Buffer
    class IndexBuffer : public Buffer
    {
    public:
        IndexBuffer(const IndexBufferDescriptor& aDescriptor, std::vector<unsigned short>* data);
        ~IndexBuffer();

        void UpdateBuffer();

        void AddIndex(const std::vector<unsigned short>& index);
        void ClearIndices();

    private:
        unsigned short* m_Buffer;
    };
}

#endif