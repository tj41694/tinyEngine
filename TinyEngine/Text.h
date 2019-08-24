#pragma once
#include "UI.h"

namespace TEngine {
	class Font;
	using namespace std;
	class Text : public UI {
	private:
		Font* font;
		wstring text;
		void Draw();
	public:
		glm::vec3 color;
		Text();
		Text(const wstring& str_);
		Text(const string& str_);
		void SetText(const wstring& str_);
		void SetText(const string& str_);
		void SetText(const float&& str_);
		void SetText(const double&& str_);
		void SetText(const int&& str_);
		void SetText(const unsigned int&& str_);
		void SetText(const long&& str_);
		void SetText(const unsigned long&& str_);
		virtual ~Text();
	};

}