#pragma once
#include "ChildWindow.h"


class DW_ToolMenu : public ChildWindow
{
public:
	DW_ToolMenu(HINSTANCE tInstance);
	void Create(HWND hParentWnd, int x, int y, int width, int height);

    bool* tCnt;
    bool pCnt;

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE tInst;
    HWND tWnd;
    RECT ToolRT;

    HWND Color1BT = nullptr;
    HWND Color2BT = nullptr;
    HWND Color3BT = nullptr;
    HWND PenWidthBT = nullptr;
    HWND ClearBT = nullptr;
    HWND SaveBT = nullptr;
    HWND PlayBT = nullptr;
    HWND ResetBT = nullptr;
};
