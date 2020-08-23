#include "SpriteAtlas.h"
#include "../Services/Services.h"
#include "../Windows/Application.h"
#include <json.h>
#include <fstream>
#include <assert.h>


namespace GameDev2D
{
    SpriteAtlas::SpriteAtlas(const std::string& aFilename) : Sprite(Services::GetResourceManager()->GetTexture(aFilename)),
        m_Filename(aFilename)
    {
        //Load the atlas frames
        Services::GetResourceManager()->LoadAtlas(m_Filename);
    }

    SpriteAtlas::~SpriteAtlas()
    {

	}

    void SpriteAtlas::UseFrame(const std::string& aAtlasKey)
    {
        //Get the atlas frame for the key
        Rect frame = Services::GetResourceManager()->GetAtlasFrame(m_Filename, aAtlasKey);
        
		if (frame.origin == Vector2::Zero && frame.size == Vector2::Zero)
		{
			//
			SetTexture(Services::GetResourceManager()->GetDefaultTexture());
		}
		else
		{
			//Set the Sprite's frame
			SetFrame(frame);
		}
    }

    bool SpriteAtlas::Unpack(const std::string& aPath, AtlasMap** aAtlasMap)
    {
        //Does the json file exist, if it doesn't the assert below will be hit
        bool doesExist = Services::GetApplication()->DoesFileExistAtPath(aPath);
        assert(doesExist == true);

        //If the json files exists, load the atlas frae
        if (doesExist == true)
        {
            //Open the input stream
            std::ifstream inputStream;
            inputStream.open(aPath.c_str(), std::ifstream::in);

            //Parse the json data
            Json::Value root;
            Json::Reader reader;
            if (reader.parse(inputStream, root, false) == true)
            {
				//
				*aAtlasMap = new AtlasMap();

                //Get the frames
                const Json::Value frames = root["frames"];

                //Cycle through the frames and parse the frame data
                for (unsigned int index = 0; index < frames.size(); ++index)
                {
                    //Safety check the filename
                    if (frames[index]["filename"].isString() == true)
                    {
                        //Get the atlas key
                        std::string atlasKey = std::string(frames[index]["filename"].asString());

                        //Load the source frame
                        Json::Value frameValue = frames[index]["frame"];

                        //Get the values from the frame
                        unsigned int x = frameValue["x"].asInt();
                        unsigned int y = frameValue["y"].asInt();
                        unsigned int width = frameValue["w"].asInt();
                        unsigned int height = frameValue["h"].asInt();

                        //Return the source frame
						Rect frame = Rect(Vector2((float)x, (float)y), Vector2((float)width, (float)height));
						(*aAtlasMap)->Create(atlasKey, frame);
                    }
                }
            }

            //Close the input stream
            inputStream.close();

            //The unpack was successful
            return true;
        }

        //The unpack failed
        return false;
    }
}