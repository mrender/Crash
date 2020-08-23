#include "VertexData.h"
#include "../Services/Services.h"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace GameDev2D
{
    VertexData::VertexData() :
        m_VertexArray(0),
        m_VertexBuffer(nullptr),
        m_IndexBuffer(nullptr)
    {
        Services::GetGraphics()->GenerateVertexArray(&m_VertexArray);
    }

    VertexData::~VertexData()
    {
        if (m_VertexBuffer != nullptr)
        {
            delete m_VertexBuffer;
            m_VertexBuffer = nullptr;
        }

        if (m_IndexBuffer != nullptr)
        {
            delete m_IndexBuffer;
            m_IndexBuffer = nullptr;
        }

        Services::GetGraphics()->DeleteVertexArray(&m_VertexArray);
    }

    VertexBuffer* VertexData::CreateBuffer(const VertexBufferDescriptor& aDescriptor, std::vector<float>* aData)
    {
        //If this assert is hit, it means you are trying to create a VertexBuffer that has already been created
        assert(m_VertexBuffer == nullptr);

        //Bind the vertex array
        Services::GetGraphics()->BindVertexArray(m_VertexArray);

        //Create and return the VertexBuffer
        m_VertexBuffer = new VertexBuffer(aDescriptor, aData);

        //Unbind the vertex array
        Services::GetGraphics()->BindVertexArray(0);

        return m_VertexBuffer;
    }

    IndexBuffer* VertexData::CreateBuffer(const IndexBufferDescriptor& aDescriptor, std::vector<unsigned short>* aData)
    {
        //If this assert is hit, it means you are trying to create a VertexBuffer that has already been created
        assert(m_IndexBuffer == nullptr);

        //Bind the vertex array
        Services::GetGraphics()->BindVertexArray(m_VertexArray);

        //Create and return the IndexBuffer
        m_IndexBuffer = new IndexBuffer(aDescriptor, aData);

        //Unbind the vertex array
        Services::GetGraphics()->BindVertexArray(0);

        return m_IndexBuffer;
    }

    VertexBuffer* VertexData::GetVertexBuffer()
    {
        return m_VertexBuffer;
    }

    IndexBuffer* VertexData::GetIndexBuffer()
    {
        return m_IndexBuffer;
    }

    void VertexData::PrepareForDraw()
    {
        //Bind the vertex array
        Services::GetGraphics()->BindVertexArray(m_VertexArray);

        //Update the vertex buffer
        if (m_VertexBuffer != nullptr)
        {
            m_VertexBuffer->UpdateBuffer();
        }

        //Update the index buffer (if one exists)
        if (m_IndexBuffer != nullptr)
        {
            m_IndexBuffer->UpdateBuffer();
        }
    }

    void VertexData::EndDraw()
    {
        //Unbind the vertex array
        Services::GetGraphics()->BindVertexArray(0);
    }


    Buffer::Buffer(BufferType aType, BufferUsage aUsage, unsigned int aSize, unsigned int aCapacity) :
        m_Type(aType),
        m_Usage(aUsage),
        m_Size(aSize),
        m_Capacity(aCapacity),
        m_Count(0),
        m_BufferId(0),
        m_IsDirty(false)
    {
        //Generate the vertex buffer and bind it
        Services::GetGraphics()->GenerateDataBuffer(&m_BufferId);
        Services::GetGraphics()->BindDataBuffer(GetType(), m_BufferId);
    }

    Buffer::~Buffer()
    {
        Services::GetGraphics()->DeleteDataBuffer(&m_BufferId);
    }

    BufferType Buffer::GetType()
    {
        return m_Type;
    }

    BufferUsage Buffer::GetUsage()
    {
        return m_Usage;
    }

    unsigned int Buffer::GetSize() 
    {
        return m_Size; 
    }

    unsigned int Buffer::GetCount() 
    { 
        return m_Count; 
    }

    unsigned int Buffer::GetCapacity() 
    { 
        return m_Capacity; 
    }


    VertexBuffer::VertexBuffer(const VertexBufferDescriptor& aDescriptor, std::vector<float>* aData) : Buffer(BufferType_Vertex, aDescriptor.usage, aDescriptor.size, aDescriptor.capacity),
        m_Buffer(nullptr),
        m_AttributeIndex(aDescriptor.attributeIndex),
        m_AttributeSize(aDescriptor.attributeSize)
    {
        //Create the buffer
        const unsigned int size = GetSize() * GetCapacity();
        m_Buffer = new float[size];

        //Is there data to initialize the buffer with
        if (aData != nullptr)
        {
            //Ensure the data size is the same size as the buffer
            assert(aData->size() == size);

            //Copy the buffer contents
            for (unsigned int i = 0; i < aData->size(); i++)
            {
                m_Buffer[i] = aData->at(i);
            }

            //Calculate the number of vertices (count) that were in the buffer
            m_Count = aData->size() / GetSize();

            //Lastly, update the vertex attributes
            UpdateAttributes();
        }

        //Initialize the buffer array
        const unsigned int numBytes = size * sizeof(float);
        Services::GetGraphics()->InitializeDataBuffer(m_BufferId, GetType(), numBytes, m_Buffer, GetUsage());
    }

    VertexBuffer::~VertexBuffer()
    {
        if (m_Buffer != nullptr)
        {
            delete[] m_Buffer;
            m_Buffer = nullptr;
        }
    }

    void VertexBuffer::UpdateBuffer()
    {
        //Are there any vertices in the VertexBuffer
        if (GetCount() == 0)
        {
            Log::Error(false, Log::Verbosity_Graphics, "[VertexBuffer] UpdateBuffer() failed, there are no vertices");
            return;
        }

        //Is the VertexBuffer dirty, do we need to update the buffer
        if (m_IsDirty == true)
        {
            //Set the buffer data
            const unsigned int numBytes = GetSize() * GetCount() * sizeof(float);
            Services::GetGraphics()->UpdateDataBuffer(m_BufferId, GetType(), 0, numBytes, m_Buffer);

            //Update the vertex attributes
            UpdateAttributes();

            //Reset the is dirty flag
            m_IsDirty = false;
        }
    }

    void VertexBuffer::UpdateAttributes()
    {
        //Ensure the attribute index and attribute size vectors have the same number of elements
        assert(m_AttributeIndex.size() == m_AttributeSize.size());

        //Locals
        const int stride = GetSize() * sizeof(float);
        unsigned int offset = 0;

        //Cycle through the attributes and set their data
        for (unsigned int i = 0; i < m_AttributeIndex.size(); i++)
        {
            //Set the attribute info
            int attributeIndex = m_AttributeIndex.at(i);
            assert(attributeIndex != -1);

            //Get the attribute size
            unsigned int attributeSize = m_AttributeSize.at(i);

            //Set the attribute data
            Services::GetGraphics()->SetVertexAttributePointer(m_BufferId, attributeIndex, attributeSize, stride, BUFFER_OFFSET(offset));

            //Increase the offset
            offset += attributeSize * sizeof(float);
        }
    }

    void VertexBuffer::AddVertex(const std::vector<float>& aVertex)
    {
        //If the size of the vertex is not the size expected, throw an assert
        assert(aVertex.size() == GetSize());

        //
        assert(m_Count < GetCapacity());

        //Add the vertex to the buffer
        for (unsigned int i = 0; i < aVertex.size(); i++)
        {
            unsigned int index = GetCount() * GetSize() + i;
            m_Buffer[index] = aVertex.at(i);
        }

        //Increment the count variable
        m_Count++;

        //Enable the dirty flag
        m_IsDirty = true;
    }

    void VertexBuffer::ClearVertices()
    {
        //Set the buffer elements to zero
        unsigned int numBytes = GetCapacity() * GetSize() * sizeof(float);
        memset(m_Buffer, 0, numBytes);

        //Reset the count to zero
        m_Count = 0;
    }

    IndexBuffer::IndexBuffer(const IndexBufferDescriptor& aDescriptor, std::vector<unsigned short>* aData) : Buffer(BufferType_Index, aDescriptor.usage, aDescriptor.size, aDescriptor.capacity),
        m_Buffer(nullptr)
    {
        //Create the buffer
        const unsigned int size = GetSize() * GetCapacity();
        m_Buffer = new unsigned short[size];

        //Is there data to initialize the buffer with
        if (aData != nullptr)
        {
            //Ensure the data size is the same size as the buffer
            assert(aData->size() == size);

            //Copy the buffer contents
            for (unsigned int i = 0; i < aData->size(); i++)
            {
                m_Buffer[i] = aData->at(i);
            }

            //Calculate the number of indicies (count) that were in the buffer
            m_Count = aData->size() / GetSize();
        }

        //Initialize the buffer array
        const unsigned int numBytes = size * sizeof(unsigned short);
        Services::GetGraphics()->InitializeDataBuffer(m_BufferId, GetType(), numBytes, m_Buffer, GetUsage());
    }

    IndexBuffer::~IndexBuffer()
    {
        if (m_Buffer != nullptr)
        {
            delete[] m_Buffer;
            m_Buffer = nullptr;
        }
    }

    void IndexBuffer::UpdateBuffer()
    {
        //Are there any vertices in the VertexBuffer
        if (GetCount() == 0)
        {
            Log::Error(false, Log::Verbosity_Graphics, "[IndexBuffer] UpdateBuffer() failed, there are no indices");
            return;
        }

        //Is the VertexBuffer dirty, do we need to update the buffer
        if (m_IsDirty == true)
        {
            //Set the buffer data
            const unsigned int numBytes = GetSize() * GetCount() * sizeof(float);
            Services::GetGraphics()->UpdateDataBuffer(m_BufferId, GetType(), 0, numBytes, m_Buffer);

            //Reset the is dirty flag
            m_IsDirty = false;
        }
    }

    void IndexBuffer::AddIndex(const std::vector<unsigned short>& aIndex)
    {
        //If the size of the index is not the size expected, throw an assert
        assert(aIndex.size() == GetSize());

        //
        assert(m_Count < GetCapacity());

        //Add the index to the buffer
        for (unsigned int i = 0; i < aIndex.size(); i++)
        {
            unsigned int index = GetCount() * GetSize() + i;
            m_Buffer[index] = aIndex.at(i);
        }

        //Increment the count variable
        m_Count++;

        //Enable the dirty flag
        m_IsDirty = true;
    }

    void IndexBuffer::ClearIndices()
    {
        //Set the buffer elements to zero
        unsigned int numBytes = GetCapacity() * GetSize() * sizeof(unsigned short);
        memset(m_Buffer, 0, numBytes);

        //Reset the count to zero
        m_Count = 0;
    }
}