#pragma once
#include "UI.h"

namespace TEngine {
	class Font;
	class Text : public UI {
	private:
		Font* font;
		std::wstring text;
		void Draw();
	public:
		glm::vec3 color;
		Text();
		Text(const std::wstring& str_);
		Text(const std::string& str_);
		void SetText(const std::wstring& str_);
		void SetText(const std::string& str_);
		void SetText(const float&& str_);
		void SetText(const double&& str_);
		void SetText(const int&& str_);
		void SetText(const unsigned int&& str_);
		void SetText(const long&& str_);
		void SetText(const unsigned long&& str_);
		virtual ~Text();
	};

}