#ifndef __GameDev2D__Polygon__
#define __GameDev2D__Polygon__

#include "GraphicTypes.h"
#include "VertexData.h"
#include "../Core/Drawable.h"
#include <vector>


namespace GameDev2D
{
    //Polygon constants
    const unsigned int POLYGON_MAX_VERTICES = 100;

    class Polygon : public Drawable
    {
    public:
        //Constructs the Polygon with zero vertices
        Polygon();
        ~Polygon();
        
        //Draws the polygon
        void Draw();
        
        //Returns the width and height of the polygon
        float GetWidth();
        float GetHeight();

		//Clears the existing vertices in the Polygon and creates a Circle
		void MakeCircle(float radius, bool isFilled, unsigned int lineSegments = 36);

		//Clears the existing vertices in the Polygon and creates a Rectangle
		void MakeRectangle(float width, float height, bool isFilled);

        //Adds a vertex to the polygon, the vertex will be added and set to the active polygon color
        void AddVertex(float x, float y);
        void AddVertex(Vector2 vertex);
        
        //Returns the number of vertices that make up the Polygon
        unsigned int GetNumberOfVertices();

        //Clears all the vertices
        void ClearVertices();

        //Sets the render mode of the Polygon
        void SetRenderMode(RenderMode renderMode);

        //Returns the render mode of the Polygon
        RenderMode GetRenderMode();

		//Override SetColor from Drawable
		void SetColor(Color color);

    private:
        //Used to calculate the size of the Polygon
        void CalculateSize();

        //Struct to hold Vertex data
        struct Vertex
        {
        public:
            Vertex(Vector2 position, Color color)
            {
                data[0] = position.x;
                data[1] = position.y;
                data[2] = color.r;
                data[3] = color.g;
                data[4] = color.b;
                data[5] = color.a;
            }

            Vector2 GetPosition() { return Vector2(data[0], data[1]); }
            Color GetColor() { return Color(data[2], data[3], data[4], data[5]); }

            const static unsigned int vertexSize = 6; //2 position, 4 color
            float data[vertexSize];
        };

        //Member variables
        VertexData* m_VertexData;
        std::vector<Vertex> m_Vertices;
        RenderMode m_RenderMode;
        Vector2 m_Size;
        bool m_EnableBlending;
    };
}

#endif
