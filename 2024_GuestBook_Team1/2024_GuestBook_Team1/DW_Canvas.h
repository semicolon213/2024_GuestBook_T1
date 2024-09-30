#ifndef DW_CANVAS_H
#define DW_CANVAS_H
#include "ChildWindow.h"
#include "ColorPalette.h"
#include "Function.h"
#include "Struct.h"

#include <memory>


class DW_Canvas : public ChildWindow
{
public:
    DW_Canvas(HINSTANCE CInstance);
    void Create(HWND hParentWnd, int x, int y, int width, int height);
protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

    std::unique_ptr<Function> function;
    std::unique_ptr<ColorPalette> colorPalette;

    HINSTANCE CInst;
    HWND CWnd;
    RECT CanvasRT;

    HDC hdc;

    PINFO drawPInfo;

    int penNum = 0;

};

#endif