#pragma once

#include "ChildWindow.h"
#include "Function.h"
#include "ColorPicker.h"

class DW_ColorBox : public ChildWindow
{
public:
	DW_ColorBox(HINSTANCE hInstance);
	void CreatePop(HWND hParentWnd, int x, int y, int width, int height);

	std::vector<PINFO>* penMemory;
protected:
	LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	HINSTANCE bInst;
	HWND bWnd;
	RECT BoxRT;
	ColorPicker* colorPicker;
};