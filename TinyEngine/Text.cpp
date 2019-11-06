#include "Text.h"
#include "Font.h"
#include <codecvt>
#include <sstream>


namespace TEngine {

	class chs_codecvt : public std::codecvt_byname<wchar_t, char, std::mbstate_t> {
	public:
		chs_codecvt() : codecvt_byname("chs") {}
	};

	Text::Text() {
		font = Font::LoadFont("resources/fonts/STXIHEI.TTF");
	}

	Text::Text(const wstring& str_) {
		font = Font::LoadFont("resources/fonts/STXIHEI.TTF");
		SetText(str_);
	}

	Text::Text(const string& str_) {
		std::wstring_convert<chs_codecvt> converter;
		std::wstring wide_string = converter.from_bytes(str_);
		font = Font::LoadFont("resources/fonts/STXIHEI.TTF");
		SetText(wide_string);
	}

	void Text::SetText(const wstring& str_) {
		text = str_;
	}
	void Text::SetText(const string& str_) {
		std::wstring_convert<chs_codecvt> converter;
		text = converter.from_bytes(str_);
	}

	void Text::SetText(const float&& str_) {
		stringstream strStream;
		strStream << str_;
		SetText(strStream.str());
	}

	void Text::SetText(const double&& str_) {
		char string[64];
		sprintf(string, "%.1f", str_);
		SetText(string);
	}

	void Text::SetText(const int&& str_) {
		stringstream strStream;
		strStream << str_;
		SetText(strStream.str());
	}

	void Text::SetText(const unsigned int&& str_) {
		stringstream strStream;
		strStream << str_;
		SetText(strStream.str());
	}

	void Text::SetText(const long&& str_) {
		stringstream strStream;
		strStream << str_;
		SetText(strStream.str());
	}

	void Text::SetText(const unsigned long&& str_) {
		stringstream strStream;
		strStream << str_;
		SetText(strStream.str());
	}

	void Text::Draw() {
		if (font)
			font->RenderText(textShader, text, WorldPos().x, WorldPos().y, scale.x, color);
		else {
			Debug::Log(font);
		}
	}

	Text::~Text() {
	}
}