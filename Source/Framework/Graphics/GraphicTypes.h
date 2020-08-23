#ifndef GameDev2D_GraphicTypes_h
#define GameDev2D_GraphicTypes_h

#include "../Math/Vector2.h"
#include <map>
#include <string>


namespace GameDev2D
{
    //Forward declarations
    class Texture;

    enum GraphicType
    {
        GraphicType_Byte = 0,
        GraphicType_UnsignedByte,
        GraphicType_Short,
        GraphicType_UnsignedShort,
        GraphicType_Int,
        GraphicType_UnsignedInt,
        GraphicType_Float,
        GraphicType_Double
    };

    enum RenderMode
    {
        RenderMode_Points = 0,
        RenderMode_Lines,
        RenderMode_LineLoop,
        RenderMode_LineStrip,
        RenderMode_Triangles,
        RenderMode_TriangleStrip,
        RenderMode_TriangleFan,
        RenderMode_Quads,
        RenderMode_QuadStrip,
        RenderMode_Polygon
    };

    enum BlendingFactor
    {
        BlendingFactor_Zero = 0,
        BlendingFactor_One,
        BlendingFactor_SourceColor,
        BlendingFactor_OneMinusSourceColor,
        BlendingFactor_SourceAlpha,
        BlendingFactor_OneMinusSourceAlpha,
        BlendingFactor_DestinationAlpha,
        BlendingFactor_OneMinusDestinationAlpha,
        BlendingFactor_DestinationColor,
        BlendingFactor_OneMinusDestinationColor,
        BlendingFactor_SourceAlphaSaturate
    };

    struct BlendingMode
    {
        BlendingMode()
        {
            source = BlendingFactor_SourceAlpha;
            destination = BlendingFactor_OneMinusSourceAlpha;
        }

        BlendingMode(BlendingFactor source, BlendingFactor destination)
        {
            this->source = source;
            this->destination = destination;
        }

        //Member variables
        BlendingFactor source;
        BlendingFactor destination;
    };

    struct PixelFormat
    {
        enum PixelLayout
        {
            RGB = 0,
            RGBA
        };

        enum PixelSize
        {
            UnsignedByte = 0,
        };

        PixelFormat(PixelLayout layout, PixelSize size) :
            layout(layout),
            size(size)
        {

        }

        PixelFormat() :
            layout(RGBA),
            size(UnsignedByte)
        {

        }

        PixelLayout layout;
        PixelSize size;
    };

    struct Wrap
    {
        enum WrapMode
        {
            ClampToEdge = 0,
            ClampToBorder,
            Repeat,
            MirroredRepeat,
            WRAP_MAX_ENUM
        };

        Wrap(WrapMode s, WrapMode t) :
            s(s),
            t(t)
        {

        }

        Wrap() :
            s(ClampToEdge),
            t(ClampToEdge)
        {

        }

        WrapMode s;
        WrapMode t;
    };

    struct ImageData
    {
        ImageData(PixelFormat format, unsigned int width, unsigned int height)
        {
            this->format = format;
            this->width = width;
            this->height = height;
            this->data = nullptr;
        }

        ImageData(PixelFormat format, unsigned int width, unsigned int height, unsigned char* data)
        {
            this->format = format;
            this->width = width;
            this->height = height;
            this->data = data;
        }

		~ImageData()
		{
			delete[] data; //FIX
		}

        //Member variables
        PixelFormat format;
        unsigned int width;
        unsigned int height;
        unsigned char* data;
    };

    struct Rect
    {
        Rect(Vector2 origin, Vector2 size)
        {
            this->origin = origin;
            this->size = size;
        }

        Rect()
        {
            origin = Vector2(0.0f, 0.0f);
            size = Vector2(0.0f, 0.0f);
        }

        bool operator==(const Rect& rect) const
        {
            return origin == rect.origin && size == rect.size;

        }

        bool operator!=(const Rect& rect) const
        {
            return origin != rect.origin || size != rect.size;

        }

        Vector2 origin;
        Vector2 size;
    };

    struct GlyphData
    {
        unsigned char width;      // width of the glyph in pixels
        unsigned char height;     // height of the glyph in pixels
        unsigned char advanceX;   // number of pixels to advance on x axis
        char bearingX;            // x offset of top-left corner from x axis
        char bearingY;            // y offset of top-left corner from y axis
        Rect frame;
    };

    struct FontData
    {
        FontData();
        ~FontData();

		Texture* texture;
        std::map<char, GlyphData> glyphData;
        std::string name;
        std::string characterSet;
        unsigned int lineHeight;
        unsigned int baseline;
        unsigned int size;
    };

    struct Viewport
    {
        Viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) :
            x(x),
            y(y),
            width(width),
            height(height)
        {

        }

        Viewport(unsigned int width, unsigned int height) :
            x(0),
            y(0),
            width(width),
            height(height)
        {

        }

        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;
    };

    enum BufferUsage
    {
        BufferUsage_StreamDraw = 0,
        BufferUsage_StreamRead,
        BufferUsage_StreamCopy,
        BufferUsage_StaticDraw,
        BufferUsage_StaticRead,
        BufferUsage_StaticCopy,
        BufferUsage_DynamicDraw,
        BufferUsage_DynamicRead,
        BufferUsage_DynamicCopy
    };

    enum BufferType
    {
        BufferType_Vertex = 0,
        BufferType_Index
    };

    struct VertexBufferDescriptor
    {
        VertexBufferDescriptor() :
            size(0),
            capacity(0),
            usage(BufferUsage_DynamicDraw)
        {
        }

        std::vector<int> attributeIndex;
        std::vector<unsigned int> attributeSize;
        unsigned int size;
        unsigned int capacity;
        BufferUsage usage;
    };

    struct IndexBufferDescriptor
    {
        IndexBufferDescriptor() :
        size(0),
        capacity(0),
        usage(BufferUsage_StaticDraw)
        {
        }

        unsigned int size;
        unsigned int capacity;
        BufferUsage usage;
    };

    enum ShaderType
    {
        VertexShader = 0,
        FragmentShader,
        GeometryShader
    };
}

#endif