#ifndef DROWWINDOW_H
#define DROWWINDOW_H
#include "ChildWindow.h"
#include "ConnExcel.h"
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

    bool toolCnt;

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE dInst;
    HWND dWnd;
    RECT drowRT;
    POINT pt;

    HDC pHdc = nullptr;
    PAINTSTRUCT d_ps = { 0 };
    HPEN partition = nullptr;
    HPEN DrowPen = nullptr;
    HBRUSH DrowBrush = nullptr;

    unique_ptr<DW_NameBar> nameBar;
    HWND nHWnd = nullptr;
    RECT DNameRT = { 0 };

    unique_ptr<DW_ToolMenu> toolMenu;
    HWND tHWnd = nullptr;
    RECT DToolRT = { 0 };


    unique_ptr<DW_Canvas> canvas;
    HWND cHWnd = nullptr;
    RECT DCanvasRT = { 0 };
    
    unique_ptr<DW_SideMenu> sideMenu;
    HWND sHWnd = nullptr;
    RECT DSideRT = { 0 };

    unique_ptr<ConnExcel> connExcel;

    RECT desktopRT;      //사용자 화면 크기 받기용
    RECT MainRT;         //메인 윈도우 크기 받기용

    int getDWWidth();

    wstring list;
    WCHAR text[10000];
};

#endif