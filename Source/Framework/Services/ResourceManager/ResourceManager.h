#ifndef __GameDev2D__ResourceManager__
#define __GameDev2D__ResourceManager__

#include "../../Audio/AudioTypes.h"
#include "../../Events/EventHandler.h"
#include "../../Graphics/Sprite.h"
#include "../../Graphics/Texture.h"
#include "../../Graphics/GraphicTypes.h"
#include <map>
#include <unordered_map>
#include <string>


//Shader constants
const std::string PASSTHROUGH_POLYGON_SHADER_KEY = "PolygonPassthrough";
const std::string PASSTHROUGH_TEXTURE_SHADER_KEY = "TexturePassthrough";
const std::string PASSTHROUGH_SPRITEBATCH_SHADER_KEY = "SpriteBatchPassthrough";

namespace GameDev2D
{
    //Templated class to make managing Resources cleaner
    template<typename T> class ResourceMap
    {
    public:
        void Create(const std::string& key, T type)
        {
            m_Map[key] = type;
        }

        void Remove(const std::string& key)
        {
            m_Map.erase(key);
        }

        T Get(const std::string& key)
        {
            return m_Map[key];
        }

        bool Contains(const std::string& key)
        {
            return m_Map.find(key) != m_Map.end();
        }

        unsigned int Count()
        {
            return m_Map.size();
        }

        void Cleanup()
        {
            for (std::map<string, T>::iterator iterator = m_Map.begin(); iterator != m_Map.end(); iterator++)
            {
                T resource = iterator->second;
                if (resource != nullptr)
                {
                    delete resource;
                    resource = nullptr;
                }
            }

            m_Map.clear();
        }

    private:
        std::map<string, T> m_Map;
    };


    //Forward declarations
    class Audio;
    class Shader;
    struct ShaderInfo;


    //The ResourceManager is responsible for loading, unloading and making accessible Audio, Font, Texture and Shader files
    class ResourceManager : public EventDispatcher
    {
    public:
        ResourceManager();
        ~ResourceManager();

        //The HandleEvent is used to notify the ResourceManager class of various GameDev2D events
        void HandleEvent(Event* event);

        //Loads an Audio object for the appropriate file, only load an Audio file once
        void LoadWaveFile(const std::string& filename);

        //Unloads an already loaded Audio object
        void UnloadWaveFile(const std::string& filename);

        //Returns wether an Audio object for the appropriate file is loaded or not
        bool IsWaveFileLoaded(const std::string& filename);

        //Returns an Audio objectfor the appropriate file, if the file doesn't exist 
        //OR isn't loaded yet then nullptr will be returned
		WaveData* GetWaveData(const std::string& filename);

        //Loads a Font for the appropriate file and font size, only load a Font once
        void LoadFont(const std::string& filename);

        //Unloads an already loaded Font
        void UnloadFont(const std::string& filename);

        //Returns wether a Shader for the appropriate file is loaded or not
        bool IsFontLoaded(const std::string& filename);

        //Returns a Font for the appropriate file and font size, if the file doesn't exist 
        //OR isn't loaded yet the default Font (OpenSans-CondBold.ttf) will be returned instead        
        FontData* GetFontData(const std::string& filename);

        //Loads a Shader for the appropriate file, only load a Shader once, the default Shaders are loaded automatically
        void LoadShader(ShaderInfo* shaderInfo, const std::string& key);

        //Unloads an already loaded Shader
        void UnloadShader(const std::string& key);

        //Returns wether a Shader for the appropriate file is loaded or not
        bool IsShaderLoaded(const std::string& key);

        //Returns a Shader for the appropriate key, if the Shader doesn't exist 
        //OR isn't loaded yet, then nullptr will be returned
        Shader* GetShader(const std::string& key);

        //Loads a Texture for the appropriate file, only load a Texture once
        void LoadTexture(const std::string& filename);

        //Unloads an already loaded Texture
        void UnloadTexture(const std::string& filename);

        //Returns wether a Texture for the appropriate file is loaded or not
        bool IsTextureLoaded(const std::string& filename);

        //Returns a Texture for the appropriate file, if the file doesn't exist 
        //OR isn't loaded yet a checkerboard Texture will be returned instead
        Texture* GetTexture(const std::string& filename);

        //Load the SpriteAtlas frames for the appropriate file is loaded or not
        void LoadAtlas(const std::string& filename);

        //Unload the SpriteAtlas frames for the appropriate file is loaded or not
        void UnloadAtlas(const std::string& filename);

        //Returns wether the SpriteAtlas frames for the appropriate file is loaded or not
        bool IsAtlasLoaded(const std::string& filename);

        //Returns a Rect for the appropriate file and atlas key, if the frame doesn't exist 
        //OR isn't loaded yet an empty Rect value will be returned instead
        Rect GetAtlasFrame(const std::string& filename, const std::string& atlasKey);

        //Returns the placeholder checkerboard texture
        Texture* GetDefaultTexture();

        //Returns the default font (OpenSans-CondBold.ttf)
		FontData* GetDefaultFont();

        //Returns a default Audio sound (1 second tone at frequency 650)
		WaveData* GetDefaultWaveData();

    private:
        //Member variables
        ResourceMap<WaveData*> m_AudioMap;
        ResourceMap<FontData*> m_FontMap;
        ResourceMap<Shader*> m_ShaderMap;
        ResourceMap<Texture*> m_TextureMap;
        ResourceMap<AtlasMap*> m_AtlasMap;
        Texture* m_DefaultTexture;
		FontData* m_DefaultFont;
		WaveData* m_DefaultAudio;
    };
}

#endif