#include "Font.h"
#include "Tools/Debug.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "Engine/UI/UI.h"

namespace TEngine {
	using namespace std;

	unordered_map<string, Font*> Font::fonts;

	Font::Font() {
	}

	bool Font::LoadCharacter(unsigned long characer) {
		GLuint glyph_index = FT_Get_Char_Index(face, characer);
		if (FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER)) {
			DEBUGLOG("ERROR::FREETYTPE: Failed to load Glyph");
			return false;
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters.insert(std::pair<unsigned long, Character>(characer, character));
		return true;
	}

	Font * Font::LoadFont(const string & path) {
		Font* font = new Font();
		if (FT_Init_FreeType(&font->ft))
			cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
		if (FT_New_Face(font->ft, path.c_str(), 0, &font->face))
			cout << "ERROR::FREETYPE: Failed to load font" << endl;
		FT_Set_Pixel_Sizes(font->face, 0, 64);  // Set size to load glyphs as
		//FT_Set_Char_Size(face, 0, 12 << 7, 300, 300);
		//FT_Select_Charmap(face, FT_ENCODING_UNICODE); // 设定为UNICODE，默认也是
		string fontName = font->face->family_name;
		if (fonts.find(fontName) == fonts.end()) {
			for (GLubyte c = 0; c < 128; c++) {  // Load first 128 characters of ASCII set
				font->LoadCharacter(c);
			}
			fonts.insert(pair<string, Font*>(fontName, font));
		} else { //如果字体存在，则删除新加载的字体
			delete font;
		}
		return fonts[fontName];
	}

	void Font::RenderText(GLuint shader, wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
		// Activate corresponding render state	
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
		glUseProgram(shader);
		glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Object::context->width), 0.0f, static_cast<GLfloat>(Object::context->height));
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(UI::UIVAO);

		for (unsigned int i = 0; i < text.size(); i++) {
			if (characters.find(text[i]) != characters.end()) {
				const Character & ch = characters[text[i]];

				GLfloat xpos = x + ch.Bearing.x * scale;
				GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				GLfloat w = ch.Size.x * scale;
				GLfloat h = ch.Size.y * scale;
				// Update VBO for each character
				GLfloat vertices[6][4] = {
					{ xpos,     ypos + h,   0.0, 0.0 },
					{ xpos,     ypos,       0.0, 1.0 },
					{ xpos + w, ypos,       1.0, 1.0 },

					{ xpos,     ypos + h,   0.0, 0.0 },
					{ xpos + w, ypos,       1.0, 1.0 },
					{ xpos + w, ypos + h,   1.0, 0.0 }
				};
				// Render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, UI::UIVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// Render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
			} else {
				LoadCharacter(text[i]);
			}
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Font::~Font() {
		// Destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		for (auto &c : characters) {
			glDeleteTextures(1, &c.second.TextureID);
		}
	}
}