#pragma once
#include "OpenglUtil.h"
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
// FreeType
#include "ft2build.h"
#include FT_FREETYPE_H

namespace TEngine {

	class Font {
	public:
		Font();
		~Font();
		void RenderText(GLuint shader, std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
		static Font* LoadFont(const std::string& path);

	private:
		struct Character {
			unsigned int TextureID;   // ID handle of the glyph texture
			glm::ivec2 Size;		  // Size of glyph
			glm::ivec2 Bearing;		  // Offset from baseline to left/top of glyph
			int Advance;			  // Horizontal offset to advance to next glyph
		};

		std::unordered_map<unsigned long, Character> characters;
		FT_Library ft = nullptr; // FreeType
		FT_Face face = nullptr;  // Load font as face
		static std::unordered_map<std::string, Font*> fonts;

	private:
		bool LoadCharacter(unsigned long characer);
	};
}