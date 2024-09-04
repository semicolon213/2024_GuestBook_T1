#pragma once
#include "ChildWindow.h"

class DW_ToolMenu : public ChildWindow
{
public:
	DW_ToolMenu(HINSTANCE tInstance);
	void Create(HWND hParentWnd, int x, int y, int width, int height);
    bool ToolCnt = TRUE;

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE tInst;
    HWND tWnd;
    RECT ToolRT;
};

