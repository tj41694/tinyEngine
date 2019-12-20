#pragma once
#include "Component.h"

namespace TEngine {
	class Script : public Component {
	public:
		Script();
		virtual ~Script();
		virtual void Start() override;
		virtual void Update() override;
	};
}