#ifndef DW_CANVAS_H
#define DW_CANVAS_H
#include "ChildWindow.h"


class DW_Canvas : public ChildWindow
{
public:
    DW_Canvas(HINSTANCE CInstance);
    void Create(HWND hParentWnd, int x, int y, int width, int height);
protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE CInst;
    HWND CWnd;
    RECT CanvasRT;
};

#endif