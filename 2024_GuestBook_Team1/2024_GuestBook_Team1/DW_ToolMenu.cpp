#include "DW_ToolMenu.h"
DW_ToolMenu::DW_ToolMenu(HINSTANCE hInstance)
	:ChildWindow(cInst, RGB(249, 249, 249))
{
	tInst = hInstance;
    ToolRT = { 0 };
    tWnd = nullptr;
}

void DW_ToolMenu::Create(HWND hParentWnd, int x, int y, int width, int height)
{
	ChildWindow::Create(hParentWnd, L"DW_ToolMenuClass", L"Tool Child Window", x, y, width, height);
	tWnd = cWnd;
}

LRESULT DW_ToolMenu::HandleMessage(HWND tWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        ToolRT = ChildWindow::GetRT();
    case WM_CREATE:



        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            

        }
        break;

    default:
        return ChildWindow::HandleMessage(tWnd, message, wParam, lParam);
    }
    return 0;
}