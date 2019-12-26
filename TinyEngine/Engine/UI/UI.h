#pragma once
#include "../Object.h"
#include <unordered_map>

namespace TEngine {

	class UI : public Object {
	public:
		static void LoadUIVAO();
		static void DrawAll();
		static GLuint UIVAO;
		static GLuint UIVBO;
		static GLuint textShader;

		UI();
		virtual ~UI();

	private:
		static std::unordered_map<unsigned int, UI*> UIs;
		virtual void Draw();
	};
}
