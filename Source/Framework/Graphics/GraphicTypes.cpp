#include "GraphicTypes.h"
#include "Texture.h"

namespace GameDev2D
{
    FontData::FontData() :
		texture(nullptr),
        lineHeight(0),
        baseline(0),
        size(0)
    {
    }

    FontData::~FontData()
    {
		if (texture != nullptr)
		{
			delete texture;
			texture = nullptr;
		}
    }

}