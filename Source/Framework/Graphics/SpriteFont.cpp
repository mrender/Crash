#include "SpriteFont.h"
#include "GraphicTypes.h"
#include "SpriteBatch.h"
#include "../Services/Services.h"
#include "../Utils/Text/Text.h"
#include <json.h>
#include <fstream>


namespace GameDev2D
{
	SpriteFont::SpriteFont(const std::string& aFilename) : Sprite(aFilename),
		m_FontData(Services::GetResourceManager()->GetFontData(aFilename)),
		m_SpriteBatch(nullptr),
		m_Text(),
		m_Size(Vector2(0.0f, 0.0f)),
		m_Justification(JustifyLeft),
		m_CharacterSpacing(0.0f)
	{
		//Set the texture for the Font
		SetTexture(Services::GetResourceManager()->GetFontData(aFilename)->texture);

		//Create the SpriteBatch
		m_SpriteBatch = new SpriteBatch();
	}

	SpriteFont::~SpriteFont()
	{
		if (m_SpriteBatch != nullptr)
		{
			delete m_SpriteBatch;
			m_SpriteBatch = nullptr;
		}
	}

	void SpriteFont::Draw()
	{
		//Is there any text to render?
		if (m_Text.length() == 0)
		{
			return;
		}

		//calculate the baseline and origin for the label
		unsigned int baseline = m_FontData->lineHeight - m_FontData->baseline;
		unsigned int numberOfLines = Text::NumberOfLines(m_Text);
		Vector2 origin = Vector2(0.0f, 0.0f);
		Vector2 position = Vector2(0.0f, 0.0f);
		position.y += baseline + (m_FontData->lineHeight * (numberOfLines - 1));
		int lineIndex = 0;

		//What justification are we dealing with
		if (m_Justification == JustifyLeft)
		{
			position.x = 0.0f;
		}
		else if (m_Justification == JustifyCenter)
		{
			position.x = (GetWidth() - m_LineWidth.at(lineIndex)) * 0.5f;
		}
		else if (m_Justification == JustifyRight)
		{
			position.x = GetWidth() - m_LineWidth.at(lineIndex);
		}

		//Begin the SpriteBatch
		m_SpriteBatch->Begin();

		//Cycle through the characters in the text label
		for (unsigned int i = 0; i < m_Text.length(); i++)
		{
			//Did we reach a new line?
			if (m_Text.at(i) == '\n')
			{
				//Increment the line index
				lineIndex++;

				//Calculate the line's origin based on the justification
				if (m_Justification == JustifyLeft)
				{
					position.x = 0.0f;
				}
				else if (m_Justification == JustifyCenter)
				{
					position.x = (GetWidth() - m_LineWidth.at(lineIndex)) * 0.5f;
				}
				else if (m_Justification == JustifyRight)
				{
					position.x = GetWidth() - m_LineWidth.at(lineIndex);
				}

				//Set the y line origin based on the line height of the font
				position.y -= m_FontData->lineHeight;
				continue;
			}

			//Set the frame
			Rect characterRect = m_FontData->glyphData[m_Text.at(i)].frame;
			SetFrame(characterRect);

			//Rect characterRect = m_Font->GetTextureRectForCharacter();
			Vector2 anchorOffset = Vector2(-GetWidth() * GetAnchor().x, -GetHeight() * GetAnchor().y);
			Vector2 charPosition = position + anchorOffset;
			charPosition.x += m_FontData->glyphData[m_Text.at(i)].bearingX;
			charPosition.y += m_FontData->glyphData[m_Text.at(i)].bearingY - characterRect.size.y;

			//Calculate the Matrix
			Matrix parent = GetTransformMatrix();
			Matrix matrix = Matrix::Make(charPosition, m_CharacterData.at(i).angle, m_CharacterData.at(i).scale);

			//Draw each character
			m_SpriteBatch->Draw(GetTexture(), parent * matrix, m_CharacterData.at(i).color, m_CharacterData.at(i).anchor, characterRect);

			//Increment the position
			position.x += m_FontData->glyphData[m_Text.at(i)].advanceX + GetCharacterSpacing();
		}

		//End the SpriteBatch
		m_SpriteBatch->End();
	}

	float SpriteFont::GetWidth()
	{
		return m_Size.x;
	}

	float SpriteFont::GetHeight()
	{
		return m_Size.y;
	}

	void SpriteFont::SetColor(Color aColor)
	{
		Drawable::SetColor(aColor);

		for (unsigned int i = 0; i < m_CharacterData.size(); i++)
		{
			m_CharacterData.at(i).color = aColor;
		}
	}

	void SpriteFont::SetText(const std::string& aText)
	{
		//Is the text the exact same, if so return
		if (aText != m_Text)
		{
			//Set the text
			m_Text = std::string(aText);

			//Clear the character data
			m_CharacterData.clear();

			//Add a CharacterData object for each letter
			for (unsigned int i = 0; i < m_Text.length(); i++)
			{
				m_CharacterData.push_back(CharacterData(GetColor()));
			}

			//Re-calculate the size
			CalculateSize();
		}
	}

	std::string SpriteFont::GetText()
	{
		return m_Text;
	}

	void SpriteFont::SetCharacterSpacing(float aCharacterSpacing)
	{
		if (m_CharacterSpacing != aCharacterSpacing)
		{
			//Set the character spacing
			m_CharacterSpacing = aCharacterSpacing;

			//Re-calculate the size
			CalculateSize();
		}
	}

	float SpriteFont::GetCharacterSpacing()
	{
		return m_CharacterSpacing;
	}

	void SpriteFont::SetJustification(SpriteFont::Justification aJustification)
	{
		//Safety check that the justification isn't already set
		if (aJustification != m_Justification)
		{
			//Set the justification
			m_Justification = aJustification;
		}
	}

	SpriteFont::Justification SpriteFont::GetJustification()
	{
		return m_Justification;
	}

	void SpriteFont::CalculateSize()
	{
		//Safety check that the font pointer is null
		if (m_FontData == nullptr)
		{
			return;
		}

		//Used to track the max width and height
		float x = 0.0f;
		Vector2 maxSize(0.0f, (float)m_FontData->lineHeight);

		//Clear the line width's vector
		m_LineWidth.clear();

		//Cycle through all the characters in the text string
		for (unsigned int i = 0; i < m_Text.length(); i++)
		{
			//Did we reach a new line?
			if (m_Text.at(i) == '\n')
			{
				//Add the line width to the line widths vector
				m_LineWidth.push_back(x);

				//Calculate the max width and increment the max height
				maxSize.x = fmaxf(x, maxSize.x);
				maxSize.y += m_FontData->lineHeight;

				//Set x back to zero
				x = 0.0f;
				continue;
			}

			//Advance the x, by the x-advance of the character
			x += m_FontData->glyphData[m_Text.at(i)].advanceX + GetCharacterSpacing();
		}

		//We are done, add the line width to the line widths vector
		m_LineWidth.push_back(x);

		//Calculate the max width
		maxSize.x = fmaxf(x, maxSize.x);

		//Set the size
		m_Size = maxSize;
	}

	std::vector<SpriteFont::CharacterData>& SpriteFont::GetCharacterData()
	{
		return m_CharacterData;
	}

	bool SpriteFont::Unpack(const std::string& aPath, FontData** aFontData)
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
				//Create the FontData object
				FontData* fontData = new FontData();
				fontData->name = root["file"].asString();
				fontData->characterSet = root["characterSet"].asString();
				fontData->size = root["size"].asUInt();
				fontData->baseline = root["baseline"].asUInt();
				fontData->lineHeight = root["lineHeight"].asUInt();

				//Get the glyphs
				const Json::Value glyphs = root["glyphs"];

				//Cycle through the frames and parse the frame data
				for (unsigned int index = 0; index < glyphs.size(); ++index)
				{
					//Safety check the filename
					if (glyphs[index]["character"].isString() == true)
					{
						//Get the character
						std::string characterStr = std::string(glyphs[index]["character"].asString());
						char character = characterStr.at(0);

						fontData->glyphData[character].advanceX = (unsigned char)glyphs[index]["advanceX"].asInt();
						fontData->glyphData[character].bearingX = (unsigned char)glyphs[index]["bearingX"].asInt();
						fontData->glyphData[character].bearingY = (unsigned char)glyphs[index]["bearingY"].asInt();

						//Load the frame
						Json::Value frameValue = glyphs[index]["frame"];
						unsigned int x = frameValue["x"].asUInt();
						unsigned int y = frameValue["y"].asUInt();
						unsigned int width = frameValue["w"].asUInt();
						unsigned int height = frameValue["h"].asUInt();

						//Set the frame
						fontData->glyphData[character].frame = Rect(Vector2((float)x, (float)y), Vector2((float)width, (float)height));
					}
				}

				*aFontData = fontData;
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