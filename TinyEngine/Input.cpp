#pragma once
#include "Input.h"
#include "Debug.h"
#include <windows.h>

namespace TEngine {
	ivec2		Input::mousePosition(0, 0);
	ivec2		Input::lasPos(0, 0);
	bool		Input::mouseMoving = false;
	long	    Input::Vertical = 0;
	long		Input::Horizontal = 0;
	double	    Input::deltaTime = 0;

	bool first = true;
	void Input::Update(ESContext *esContext_, const float deltaTime_) {
		HWND hWnd = GetActiveWindow();
		if (hWnd == esContext_->eglNativeWindow) {
			//UINT dwSize = 40;
			//static BYTE lpb[40];

			//GetRawInputData(0, RID_INPUT,
			//	lpb, &dwSize, sizeof(RAWINPUTHEADER));

			//RAWINPUT* raw = (RAWINPUT*)lpb;

			//if (raw->header.dwType == RIM_TYPEMOUSE) {
			//	int xPosRelative = raw->data.mouse.lLastX;
			//	int yPosRelative = raw->data.mouse.lLastY;
			//}

			if (esContext_->eglContext)
				deltaTime = deltaTime_;
			POINT      point;
			GetPhysicalCursorPos(&point);
			if (first) {
				mousePosition.x = point.x;
				mousePosition.y = point.y;
				lasPos.x = mousePosition.x;
				lasPos.y = mousePosition.y;
				first = false;
			}
			Vertical = lasPos.y - mousePosition.y;
			Horizontal = lasPos.x - mousePosition.x;
			lasPos.x = mousePosition.x;
			lasPos.y = mousePosition.y;
			mousePosition.x = point.x;
			mousePosition.y = point.y;
			mouseMoving = (lasPos != mousePosition);
		}
	}

	bool Input::GetKey(int vKey) {
		return GetAsyncKeyState(vKey);
	}

	bool Input::GetKeyDown(int vKey) {
		return GetAsyncKeyState(vKey) == -32767;
	}

	long Input::GetAxisX() {
		return Horizontal;
	}

	long Input::GetAxisY() {
		return Vertical;
	}

}