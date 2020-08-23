#include "ResourceManager.h"
#include "../../Audio/Audio.h"
#include "../../Audio/AudioTypes.h"
#include "../../Debug/Log.h"
#include "../../Events/TextureResourceEvent.h"
#include "../../Graphics/Shader.h"
#include "../../Graphics/SpriteAtlas.h"
#include "../../Graphics/SpriteFont.h"
#include "../../IO/File.h"
#include "../../Utils/Png/Png.h"
#include "../../Utils/Wave/Wave.h"
#include "../../Windows/Application.h"
#include "../Services.h"



namespace GameDev2D
{
    ResourceManager::ResourceManager() : EventDispatcher(),
        m_DefaultTexture(nullptr),
        m_DefaultFont(nullptr),
        m_DefaultAudio(nullptr)
    {
        //Initialize the attributes for the polygon shader
        vector<string> attributes;
        attributes.push_back("in_vertices");
        attributes.push_back("in_color");

        //Initialize the uniforms for the polygon shader
        vector<string> uniforms;
        uniforms.push_back("uniform_anchor");
        uniforms.push_back("uniform_size");
        uniforms.push_back("uniform_model");
        uniforms.push_back("uniform_view");
        uniforms.push_back("uniform_projection");

        //Load the pass through shader
        ShaderInfo passthrough = ShaderInfo("passThrough", "passThrough", attributes, uniforms);
        LoadShader(&passthrough, PASSTHROUGH_POLYGON_SHADER_KEY);

        //Re-initialize the attributes for the Texture shader
        attributes.clear();
        attributes.push_back("in_vertices");
        attributes.push_back("in_uv");

        //Re-initialize the uniforms for the Texture shader
        uniforms.clear();
        uniforms.push_back("uniform_anchor");
        uniforms.push_back("uniform_size");
        uniforms.push_back("uniform_color");
        uniforms.push_back("uniform_texture");
        uniforms.push_back("uniform_model");
        uniforms.push_back("uniform_view");
        uniforms.push_back("uniform_projection");

        //Load the texture pass through shader
        ShaderInfo texturePassthrough = ShaderInfo("passThrough-texure", "passThrough-texure", attributes, uniforms);
        LoadShader(&texturePassthrough, PASSTHROUGH_TEXTURE_SHADER_KEY);

        //Re-initialize the attributes for the SpriteBatch shader
        attributes.clear();
        attributes.push_back("in_vertices");
        attributes.push_back("in_uv");
        attributes.push_back("in_color");

        //Re-initialize the uniforms for the SpriteBatch shader
        uniforms.clear();
        uniforms.push_back("uniform_texture");
        uniforms.push_back("uniform_model");
        uniforms.push_back("uniform_view");
        uniforms.push_back("uniform_projection");

        //Load the texture pass through shader
        ShaderInfo spriteBatchPassthrough = ShaderInfo("passThrough-spriteBatch", "passThrough-spriteBatch", attributes, uniforms);
        LoadShader(&spriteBatchPassthrough, PASSTHROUGH_SPRITEBATCH_SHADER_KEY);

        //Add an event listener callback for the Update event
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

    ResourceManager::~ResourceManager()
    {
        //Delete the default Texture
        if (m_DefaultTexture != nullptr)
        {
            delete m_DefaultTexture;
            m_DefaultTexture = nullptr;
        }

        //Delete the default font
        if (m_DefaultFont != nullptr)
        {
            delete m_DefaultFont;
            m_DefaultFont = nullptr;
        }

        //Delete the default Audio
        if (m_DefaultAudio != nullptr)
        {
            delete m_DefaultAudio;
            m_DefaultAudio = nullptr;
        }

        //Unload the default Shaders
        UnloadShader(PASSTHROUGH_POLYGON_SHADER_KEY);
        UnloadShader(PASSTHROUGH_TEXTURE_SHADER_KEY);
        UnloadShader(PASSTHROUGH_SPRITEBATCH_SHADER_KEY);

        //Check how many Textures remain loaded
        unsigned int texturesLeft = m_TextureMap.Count();
        if (texturesLeft > 0)
        {
            //Log that there are textures left
            Log::Error(false, Log::Verbosity_Resources, "[ResourceManager] %u Textures remain unloaded", texturesLeft);

            //Then cleanup the left over textures
            m_TextureMap.Cleanup();
        }

        //Check how many Fonts remain loaded
        unsigned int fontsLeft = m_FontMap.Count();
        if (fontsLeft > 0)
        {
            //Log that there are fonts left
            Log::Error(false, Log::Verbosity_Resources, "[ResourceManager] %u Fonts remain unloaded", fontsLeft);

            //Then cleanup the left over fonts
            m_FontMap.Cleanup();
        }

        //Check how many Sounds remain loaded
        unsigned int soundsLeft = m_AudioMap.Count();
        if (soundsLeft > 0)
        {
            //Log that there are sounds left
            Log::Error(false, Log::Verbosity_Resources, "[ResourceManager] %u Audio files remain unloaded", soundsLeft);

            //Then cleanup the left over sounds
            m_AudioMap.Cleanup();
        }

		//Cleanup the atlas map
		m_AtlasMap.Cleanup();

        //Remove the event listener callback for the Update event
        Services::GetApplication()->RemoveEventListener(this, UPDATE_EVENT);
    }

    void ResourceManager::HandleEvent(Event* aEvent)
    {
        if (aEvent != nullptr)
        {
            if (aEvent->GetEventCode() == UPDATE_EVENT)
            {

            }
        }
    }

    void ResourceManager::LoadWaveFile(const std::string& aFilename)
    {
        if (IsWaveFileLoaded(aFilename) == false)
        {
            //Safety check the filename
            if (aFilename.length() == 0)
            {
                Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load wave file, the filename had a length of 0");
                return;
            }

			//Append the filename to the directory
			string filename = string(aFilename);

			//Was .wav appended to the filename? If it was, remove it
			size_t found = filename.find(".wav");
			if (found != std::string::npos)
			{
				filename.erase(found, 4);
			}

            //Get the path for the audio file
            string path = Services::GetApplication()->GetPathForResourceInDirectory(filename.c_str(), "wav", "Audio");

            //Does the file exist, if it doesn't the assert below will be hit
            bool doesExist = Services::GetApplication()->DoesFileExistAtPath(path);
            assert(doesExist == true);

            //If the file doesn't exist, log an error
            if (doesExist == false)
            {
                Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load wave file with filename: %s.wav, it doesn't exist", filename.c_str());
                return;
            }

			WaveData* waveData = nullptr;
			if (Wave::LoadFromPath(path, &waveData) == true)
			{
				//Create the audio data and add it to the resource map
				m_AudioMap.Create(filename, waveData);
			}
			else
			{
				Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load wave file with filename: %s.wav", filename.c_str());
			}
        }
    }

    void ResourceManager::UnloadWaveFile(const std::string& aFilename)
    {
        //Is the wave data loaded?
        if (IsWaveFileLoaded(aFilename) == true)
        {
            //Get the audio resource
            WaveData* resource = m_AudioMap.Get(aFilename);

            //Delete the resource
            if (resource != nullptr)
            {
                delete resource;
                resource = nullptr;
            }

            //Remove the resource from the wave data map
            m_AudioMap.Remove(aFilename);
        }
    }

    bool ResourceManager::IsWaveFileLoaded(const std::string& aFilename)
    {
        return m_AudioMap.Contains(aFilename);
    }

    WaveData* ResourceManager::GetWaveData(const std::string& aFilename)
    {
        //Is the audio data loaded?
        if (IsWaveFileLoaded(aFilename) == true)
        {
            WaveData* audioData = m_AudioMap.Get(aFilename);
            return audioData;
        }
		return GetDefaultWaveData();
    }
    
    void ResourceManager::LoadFont(const std::string& aFilename)
    {
        //Check if the font loaded
        if (IsFontLoaded(aFilename) == false)
        {
            //Safety check the filename
            if (aFilename.length() == 0)
            {
                Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load font, the filename had a length of 0");
                return;
            }

			//Get the path for the JSON file
			std::string path = Services::GetApplication()->GetPathForResourceInDirectory(aFilename.c_str(), "json", "Fonts");

			//Unpack the SpriteFont Data
			FontData* fontData = nullptr;
			bool success = SpriteFont::Unpack(path, &fontData);
			
			if (success == true && fontData != nullptr)
			{
				//Get the path for the texture
				std::string path = Services::GetApplication()->GetPathForResourceInDirectory(aFilename.c_str(), "png", "Fonts");

				//Does the image exist at the path
				if (Services::GetApplication()->DoesFileExistAtPath(path) == true)
				{
					//Attempt to load the png image and store its texture data in the TextureData struct
					ImageData* imageData = nullptr;
					bool success = Png::LoadFromPath(path, &imageData);

					//Did the image load successfully
					if (success == true && imageData != nullptr)
					{
						//Create a new texture object
						fontData->texture = new Texture(*imageData);

						//Set the fontData in the font map
						m_FontMap.Create(aFilename, fontData);
					}
					else
					{
						//Log an error message
						Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load font : %s", aFilename.c_str());
					}

					//Delete the imageData, we don't need it anymore
					if (imageData != nullptr)
					{
						delete imageData;
						imageData = nullptr;
					}
				}
			}
			else
			{
				//Log an error message
				Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load font : %s", aFilename.c_str());
			}
        }
    }

    void ResourceManager::UnloadFont(const std::string& aFilename)
    {
        //Is the Font loaded?
        if (IsFontLoaded(aFilename) == true)
        {
            //Delete the resource
            FontData* resource = m_FontMap.Get(aFilename);
            if (resource != nullptr)
            {
                delete resource;
                resource = nullptr;
            }

            //Remove the resource from the Font map
            m_FontMap.Remove(aFilename);
        }
    }

    bool ResourceManager::IsFontLoaded(const std::string& aFilename)
    {
        return m_FontMap.Contains(aFilename);
    }

    FontData* ResourceManager::GetFontData(const std::string& aFilename)
    {
        //Check to see if the font is even loaded
        if (IsFontLoaded(aFilename) == true)
        {
            return m_FontMap.Get(aFilename);
        }

		return GetDefaultFont();
    }

    void ResourceManager::LoadShader(ShaderInfo* aShaderInfo, const string& aKey)
    {
        //Is the Shader loaded?
        if (IsShaderLoaded(aKey) == false)
        {
            //Safety check the filenames
            if (aShaderInfo->vertex.length() == 0 || aShaderInfo->fragment.length() == 0)
            {
                if (aShaderInfo->vertex.length() == 0)
                {
                    Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load the Shader %s, the .vsh shader file had a length of 0", aKey.c_str());
                }

                if (aShaderInfo->fragment.length() == 0)
                {
                    Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load the Shader %s, the .fsh shader file had a length of 0", aKey.c_str());
                }
                return;
            }

            //Was .vsh appended to the filename? If it was, remove it
            string vertexShader = string(aShaderInfo->vertex);
            size_t found = vertexShader.find(".vsh");
            if (found != std::string::npos)
            {
                vertexShader.erase(found, 4);
            }

            //Get the path for the vertex shader file
            string vertexPath = Services::GetApplication()->GetPathForResourceInDirectory(vertexShader.c_str(), "vsh", "Shaders");

            //Load the vertex shader
            File vertexShaderFile(vertexPath);

            //Safety check the vertex shader
            if (vertexShaderFile.GetBufferSize() == 0)
            {
                Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load the vertex shader %s", aKey.c_str());
                return;
            }

            //Was .fsh appended to the filename? If it was, remove it
            string fragmentShader = string(aShaderInfo->fragment);
            found = fragmentShader.find(".fsh");
            if (found != std::string::npos)
            {
                fragmentShader.erase(found, 4);
            }

            //Get the path for the fragment shader file
            string fragmentPath = Services::GetApplication()->GetPathForResourceInDirectory(fragmentShader.c_str(), "fsh", "Shaders");

            //Load the fragment shader
            File fragmentShaderFile(fragmentPath);

            //Safety check the fragment shader
            if (fragmentShaderFile.GetBufferSize() == 0)
            {
                Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load the fragment shader %s", aKey.c_str());
                return;
            }

            //Declare the shader pointer
            Shader* shader = nullptr;

            //Is there a geometry shader?
            if (aShaderInfo->geometry.length() > 0)
            {
                //Was .gsh appended to the filename? If it was, remove it
                string geometryShader = string(aShaderInfo->geometry);
                found = geometryShader.find(".gsh");
                if (found != std::string::npos)
                {
                    geometryShader.erase(found, 4);
                }

                //Get the path for the geometry shader file
                string geometryPath = Services::GetApplication()->GetPathForResourceInDirectory(geometryShader.c_str(), "gsh", "Shaders");

                //Load the geometry shader
                File geometryShaderFile(geometryPath);

                //Safety check the geometry shader
                if (geometryShaderFile.GetBufferSize() == 0)
                {
                    Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load the geometry shader %s", aKey.c_str());
                    return;
                }

                //Create a new shader with the vertex and fragment shaders
                ShaderSource shaderSource = ShaderSource(vertexShaderFile.GetBuffer(), fragmentShaderFile.GetBuffer(), geometryShaderFile.GetBuffer());
                shader = new Shader(shaderSource);
            }
            else
            {
                //Create a new shader with the vertex and fragment shaders
                ShaderSource shaderSource = ShaderSource(vertexShaderFile.GetBuffer(), fragmentShaderFile.GetBuffer());
                shader = new Shader(shaderSource);
            }

            //The shader SHOULDN'T be null
            assert(shader != nullptr);

            //Set the shader key
            shader->SetKey(aKey);

            //Set the shader map pair for the filename key
            m_ShaderMap.Create(aKey, shader);

            //Cycle through the attributes and add them to the shader
            for (unsigned int i = 0; i < aShaderInfo->attributes.size(); i++)
            {
                shader->AddAttribute(aShaderInfo->attributes.at(i));
            }

            //Cycle through the uniforms and add them to the shader
            for (unsigned int i = 0; i < aShaderInfo->uniforms.size(); i++)
            {
                shader->AddUniform(aShaderInfo->uniforms.at(i));
            }
        }
    }

    void ResourceManager::UnloadShader(const string& aKey)
    {
        //Is the Shader loaded?
        if (IsShaderLoaded(aKey) == true)
        {
            //Get the Shader
            Shader* resource = m_ShaderMap.Get(aKey);

            //Delete the Shader
            if (resource != nullptr)
            {
                delete resource;
                resource = nullptr;
            }

            //Remove the resource from the Shader map
            m_ShaderMap.Remove(aKey);
        }
    }

    bool ResourceManager::IsShaderLoaded(const string& aKey)
    {
        return m_ShaderMap.Contains(aKey);
    }

    Shader* ResourceManager::GetShader(const string& aKey)
    {
        if (m_ShaderMap.Contains(aKey) == true)
        {
            return m_ShaderMap.Get(aKey);
        }

        return m_ShaderMap.Get(PASSTHROUGH_TEXTURE_SHADER_KEY);
    }

#include "../../Debug/Profile.h"

    void ResourceManager::LoadTexture(const string& aFilename)
    {
        //Is the Texture loaded?
        if (IsTextureLoaded(aFilename) == false)
        {
            //Safety check the filename
            if (aFilename.length() == 0)
            {
                Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load texture, the filename had a length of 0");
                return;
            }

            //Append the filename to the directory
            string filename = string(aFilename);

            //Was .png appended to the filename? If it was, remove it
            size_t found = filename.find(".png");
            if (found != std::string::npos)
            {
                filename.erase(found, 4);
            }

            //Get the path for the texture
            std::string path = Services::GetApplication()->GetPathForResourceInDirectory(filename.c_str(), "png", "Images");

			//Does the image exist at the path
            if (Services::GetApplication()->DoesFileExistAtPath(path) == true)
            {
                //Attempt to load the png image and store its texture data in the TextureData struct
                ImageData* imageData = nullptr;
                bool success = Png::LoadFromPath(path, &imageData);

                //Did the image load successfully
                if (success == true && imageData != nullptr)
                {
                    //Create a new texture object
                    Texture* texture = new Texture(*imageData);

					//Dispatch an event before the resource is deleted
					DispatchEvent(TextureResourceEvent(texture, TEXTURE_RESOURCE_LOADED));

                    //Create the entry for the textureData
                    m_TextureMap.Create(aFilename, texture);
                }
                else
                {
                    //Log an error message
                    Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load texture : %s", aFilename.c_str());
                }

                //Delete the imageData, we don't need it anymore
                if (imageData != nullptr)
                {
                    delete imageData;
                    imageData = nullptr;
                }
            }
        }
    }

    void ResourceManager::UnloadTexture(const string& aFilename)
    {
        //Is the Texture loaded
        if (IsTextureLoaded(aFilename) == true)
        {
			//Get the resource
			Texture* resource = m_TextureMap.Get(aFilename);

			//Dispatch an event before the resource is deleted
			DispatchEvent(TextureResourceEvent(resource, TEXTURE_RESOURCE_UNLOADED));

            //Delete the resource
            if (resource != nullptr)
            {
                delete resource;
                resource = nullptr;
            }

            //Remove the resource from the Texture map
            m_TextureMap.Remove(aFilename);
        }
    }

    bool ResourceManager::IsTextureLoaded(const string& aFilename)
    {
        return m_TextureMap.Contains(aFilename);
    }

    Texture* ResourceManager::GetTexture(const string& aFilename)
    {
        //Set the texture data
        if (IsTextureLoaded(aFilename) == true)
        {
            return m_TextureMap.Get(aFilename);
        }

        //If the texture still isn't loaded, it doesn't exist, set 
        //the default texture data instead
        return GetDefaultTexture();
    }

    void ResourceManager::LoadAtlas(const string& aFilename)
    {
        if (IsAtlasLoaded(aFilename) == false)
        {
            //Get the json path
            string jsonPath = Services::GetApplication()->GetPathForResourceInDirectory(aFilename.c_str(), "json", "Images");

			//Check to see if the file exists
			if (Services::GetApplication()->DoesFileExistAtPath(jsonPath) == true)
			{
				//Unpack the sprite atlas
				AtlasMap* atlasMap = nullptr;
				SpriteAtlas::Unpack(jsonPath, &atlasMap);

				//Add the unpacked sprite atlas data to the ResourceMap
				if (atlasMap != nullptr)
				{
					m_AtlasMap.Create(aFilename, atlasMap);
				}
			}
        }
    }

    void ResourceManager::UnloadAtlas(const string& aFilename)
    {
        if (IsAtlasLoaded(aFilename) == true)
        {
            m_AtlasMap.Remove(aFilename);
        }
    }

    bool ResourceManager::IsAtlasLoaded(const string& aFilename)
    {
        return m_AtlasMap.Contains(aFilename);
    }

    Rect ResourceManager::GetAtlasFrame(const string& aFilename, const string& aAtlasKey)
    {
        //Ensure this time that the atlas data has been loaded
        if (IsAtlasLoaded(aFilename) == true)
        {
            //Get the specific TextureFrame from the atlas ResourceMap
			AtlasMap* atlasMap = m_AtlasMap.Get(aFilename);
			if (atlasMap != nullptr)
			{
				return atlasMap->Get(aAtlasKey);
			}
        }

        return Rect();
    }

    Texture* ResourceManager::GetDefaultTexture()
    {
        if (m_DefaultTexture == nullptr)
        {
            //Local constants
            const int placeHolderSize = 64;  //Width and Height
            const int colorSize = 4;  //RGBA

            //Local variables used below
            int checkerSize = placeHolderSize >> 1;
            int rowBytes = placeHolderSize * colorSize;
            Color colorA(0x555555ff);
            Color colorB(0xbbbbbbff);

            //Create our image buffer
            unsigned int dataSize = colorSize * placeHolderSize * placeHolderSize;
            unsigned char* data = new unsigned char[dataSize];

            //Cycle through and set the checkered board pattern
            int index = 0;
            for (int i = 0; i < checkerSize; ++i)
            {
                for (int j = 0; j < checkerSize; ++j)
                {
                    //Red
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.r * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.r * 255.0f);
                    index++;

                    //Green
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.g * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.g * 255.0f);
                    index++;

                    //Blue
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.b * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.b * 255.0f);
                    index++;

                    //Alpha
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.a * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.a * 255.0f);
                    index++;
                }
                index += checkerSize * colorSize;
            }

            //Create the texture object
            PixelFormat format = PixelFormat(PixelFormat::RGBA, PixelFormat::UnsignedByte);
            ImageData imageData = ImageData(format, placeHolderSize, placeHolderSize, data);
            m_DefaultTexture = new Texture(imageData);
        }

        //Return the default texture
        return m_DefaultTexture;
    }

    FontData* ResourceManager::GetDefaultFont()
    {
        if (m_DefaultFont == nullptr)
        {
            //Get the path for the JSON file
            std::string path = Services::GetApplication()->GetPathForResourceInDirectory("OpenSans-CondBold_32", "json", "Fonts");

            //Unpack the SpriteFont Data
            bool success = SpriteFont::Unpack(path, &m_DefaultFont);
            if (success == true && m_DefaultFont != nullptr)
            {
                //Get the path for the texture
                std::string path = Services::GetApplication()->GetPathForResourceInDirectory("OpenSans-CondBold_32", "png", "Fonts");

                //Does the image exist at the path
                if (Services::GetApplication()->DoesFileExistAtPath(path) == true)
                {
                    //Attempt to load the png image and store its texture data in the TextureData struct
                    ImageData* imageData = nullptr;
                    bool success = Png::LoadFromPath(path, &imageData);

                    //Did the image load successfully
                    if (success == true && imageData != nullptr)
                    {
                        //Create a new texture object
                        m_DefaultFont->texture = new Texture(*imageData);
                    }
                    else
                    {
                        //Delete the default font
                        delete m_DefaultFont;
                        m_DefaultFont = nullptr;

                        //Log an error message
                        Log::Error(false, Log::Verbosity_Resources, "[Resource Manager] Failed to load default font");
                    }

                    //Delete the imageData, we don't need it anymore
                    if (imageData != nullptr)
                    {
                        delete imageData;
                        imageData = nullptr;
                    }
                }
            }
            else
            {
                m_DefaultFont = nullptr;
            }
        }

        return m_DefaultFont;
    }

	WaveData* ResourceManager::GetDefaultWaveData()
    {
        //If the default WaveData object isn't created, create it, then return it
        if (m_DefaultAudio == nullptr)
        {
            //Local variables
            const unsigned int numChannels = 1;       //Mono
			const unsigned int duration = 2;       //In seconds;
			const unsigned int sampleRate = 8192;
			const unsigned int frequency = 650;
			const unsigned int frames = sampleRate * duration;

			//Create the WaveData object
			m_DefaultAudio = new WaveData();

			//Initialize the wave format struct
			m_DefaultAudio->waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			m_DefaultAudio->waveFormat.nChannels = numChannels;
			m_DefaultAudio->waveFormat.nSamplesPerSec = sampleRate;
			m_DefaultAudio->waveFormat.nAvgBytesPerSec = sampleRate * sizeof(short);
			m_DefaultAudio->waveFormat.nBlockAlign = sizeof(short);
			m_DefaultAudio->waveFormat.wBitsPerSample = 16; //16 bits in a short
			m_DefaultAudio->waveFormat.cbSize = 0; 

			//Create the data array
			short* shortBuffer = new short[frames];
			m_DefaultAudio->data = (void*)shortBuffer;

			//Fill the data array with a tone generated from a sine wave
			for (unsigned int i = 0; i < frames; i++)
			{
			    ((short *)m_DefaultAudio->data)[i] = (short)(SHRT_MAX * sin((2 * M_PI * frequency) / sampleRate * i));
			}

			//Fill the audio buffer struct
			m_DefaultAudio->buffer.AudioBytes = frames * sizeof(short);
			m_DefaultAudio->buffer.pAudioData = (BYTE*)m_DefaultAudio->data;
			m_DefaultAudio->buffer.PlayBegin = 0;
			m_DefaultAudio->buffer.PlayLength = 0;
        }

		//Return the default audio data
        return m_DefaultAudio;
    }
}