#pragma once
#include "ChildWindow.h"
#include "DW_ToolMenu.h"

class DrowWindow : public ChildWindow 
{
public:
    DrowWindow(HINSTANCE bInstance);
    void Create(HWND hParentWnd, int x, int y, int width, int height);

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE dInst;
    HWND dWnd;
    RECT DrowRT;
};

