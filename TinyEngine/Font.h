#pragma once
#include "OpenglUtil.h"
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
// FreeType
#include "ft2build.h"
#include FT_FREETYPE_H

namespace TEngine {
	using namespace std;
	class Font {
	private:
		struct Character {
			unsigned int TextureID;   // ID handle of the glyph texture
			glm::ivec2 Size;		  // Size of glyph
			glm::ivec2 Bearing;		  // Offset from baseline to left/top of glyph
			int Advance;     // Horizontal offset to advance to next glyph
		};

		static unordered_map<string, Font*> fonts;

		FT_Library ft; // FreeType
		FT_Face face;  // Load font as face
		Font();
		bool LoadCharacter(unsigned long characer);
	public:
		static Font* LoadFont(const string& path);

		unordered_map<unsigned long, Character> characters;
		void RenderText(GLuint shader, wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
		~Font();
	};
}