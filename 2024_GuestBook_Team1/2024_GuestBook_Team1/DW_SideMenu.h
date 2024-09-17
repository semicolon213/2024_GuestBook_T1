#pragma once
#include "ChildWindow.h"


class DW_SideMenu : public ChildWindow
{
public:
    DW_SideMenu(HINSTANCE sInstance);
    void Create(HWND hParentWnd, int x, int y, int width, int height);

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE sInst;
    HWND sWnd;
    RECT SideRT;


    HWND NewFile;
    HWND SaveFile;
    HWND LoadFile;
    HWND FileManager;
    HWND Credit;
};

