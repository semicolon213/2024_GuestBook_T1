#pragma once
#include "ChildWindow.h"


class DW_NameBar : public ChildWindow
{
public:
    DW_NameBar(HINSTANCE nInstance);
    void Create(HWND hParentWnd, int x, int y, int width, int height);
    bool* tCnt;

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE nInst;
    HWND nWnd;
    RECT NameRT;


    HWND BackB;
    HWND SideB;
    HWND FileNameW;


};

