#pragma once
#include "Script.h"

	class BoardCtr :
		public Script {
	public:
		BoardCtr(Object* obj);
		void Start();
		void Update();
		virtual ~BoardCtr();
	};