#pragma once
#include "ChildWindow.h"
#include "DW_ToolMenu.h"
#include "DW_Canvas.h"
#include "DW_NameBar.h"
#include "DW_SideMenu.h"

#include "Resource.h"


class DrowWindow : public ChildWindow 
{
public:
    DrowWindow(HINSTANCE bInstance);
    void Create(HWND hParentWnd, int x, int y, int width, int height);

    bool ToolCnt;

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE dInst;
    HWND dWnd;
    RECT DrowRT;

    DW_SideMenu* SideMenu;
    HWND s_hWnd;
};
