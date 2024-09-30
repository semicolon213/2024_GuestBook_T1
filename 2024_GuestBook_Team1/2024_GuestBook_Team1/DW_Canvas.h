#ifndef DW_CANVAS_H
#define DW_CANVAS_H
#include "ChildWindow.h"
#include "ColorPalette.h"
#include "Function.h"
#include "Struct.h"
#include "PenThickness.h"



class DW_Canvas : public ChildWindow
{
public:
    DW_Canvas(HINSTANCE CInstance);
    void Create(HWND hParentWnd, int x, int y, int width, int height);

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

    std::unique_ptr<Function> function;
    std::unique_ptr<ColorPalette> colorPalette;
    std::unique_ptr<PenThickness> penThickness;

    HINSTANCE cInst;
    HWND canWnd;
    RECT canvasRT;

    HDC hdc;

    PINFO drawPInfo;

};

#endif