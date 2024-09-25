#ifndef DROWWINDOW_H
#define DROWWINDOW_H
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
    POINT pt;

    HDC pHdc = nullptr;
    PAINTSTRUCT d_ps = { 0 };
    HPEN partition = nullptr;
    HPEN DrowPen = nullptr;
    HBRUSH DrowBrush = nullptr;


    DW_SideMenu* SideMenu = nullptr;
    HWND s_hWnd = nullptr;
    RECT DSideRT = { 0 };


    DW_NameBar* NameBar = nullptr;
    HWND n_hWnd = nullptr;
    RECT DNameRT = { 0 };

    DW_ToolMenu* ToolMenu = nullptr;
    HWND t_hWnd = nullptr;
    RECT DToolRT = { 0 };


    DW_Canvas* Canvas = nullptr;
    HWND C_hWnd = nullptr;
    RECT DCanvasRT = { 0 };
    
    RECT DesktopRT;      //����� ȭ�� ũ�� �ޱ��
    RECT MainRT;         //���� ������ ũ�� �ޱ��
};

#endif