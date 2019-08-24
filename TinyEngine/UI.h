#pragma once
#include "Object.h"
#include <unordered_map>

namespace TEngine {

	class UI : public Object {
	private:
		static std::unordered_map<unsigned int, UI*> UIs;
		virtual void Draw();
	public:
		static void LoadUIVAO();
		static GLuint UIVAO;
		static GLuint UIVBO;
		static GLuint textShader;
		UI();
		static void DrawAll();
		virtual ~UI();
	};
}
