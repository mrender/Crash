#pragma once

#include "SpriteAtlas.h"
#include "GraphicTypes.h"


namespace GameDev2D
{
	class SpriteBatch;

	class SpriteFont : public Sprite
	{
	public:
		//A font should only be generated through the ResourceManager
		SpriteFont(const std::string& filename);
		~SpriteFont();

		//Draws the Label
		void Draw();

		//Returns the size of the label
		float GetWidth();
		float GetHeight();

		//Sets the Color of all the characters
		void SetColor(Color color);

		//Sets the text that the Label will display
		void SetText(const std::string& text);

		//Returns the text the Label is displaying
		std::string GetText();

		//Set the spacing between each character
		void SetCharacterSpacing(float characterSpacing);

		//Returns the spacing between each character
		float GetCharacterSpacing();

		//Label Justification text can be left, center and right justified
		enum Justification
		{
			JustifyLeft = 0,
			JustifyCenter,
			JustifyRight
		};

		//Sets the justification of the Label
		void SetJustification(Justification justification);

		//Returns the justification of the Label
		Justification GetJustification();

		//
		struct CharacterData
		{
			CharacterData(Color color) :
				color(color),
				scale(1.0f, 1.0f),
				anchor(0.0f, 0.0f),
				angle(0.0f)
			{}

			CharacterData() :
				color(Color::WhiteColor()),
				scale(1.0f, 1.0f),
				anchor(0.0f, 0.0f),
				angle(0.0f)
			{}

			CharacterData(const CharacterData& characterData) :
				color(characterData.color),
				scale(characterData.scale),
				anchor(characterData.anchor),
				angle(characterData.angle)
			{}

			Color color;
			Vector2 scale;
			Vector2 anchor;
			float angle;
		};

		//Returns a reference to the CharacterData so that you can modify each character's data individually
		std::vector<CharacterData>& GetCharacterData();

		//Unpacks the Atlas .json file
		static bool Unpack(const std::string& path, FontData** fontData);

	private:
		//Conveniance method to calculate the size of the SpriteFont, based on the text
		void CalculateSize();

		//Member variables
		FontData* m_FontData;
		SpriteBatch* m_SpriteBatch;
		std::string m_Text;
		std::vector<float> m_LineWidth;
		std::vector<CharacterData> m_CharacterData;
		Vector2 m_Size;
		Justification m_Justification;
		float m_CharacterSpacing;
	};
}