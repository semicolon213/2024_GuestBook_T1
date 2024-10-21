#ifndef DW_TOOLMENU_H
#define DW_TOOLMENU_H
#include "ChildWindow.h"
#include "Function.h"
#include <thread>

#include "ColorPalette.h"
#include "MakeButton.h"


class DW_ToolMenu : public ChildWindow
{
public:
	DW_ToolMenu(HINSTANCE tInstance);
	void Create(HWND hParentWnd, int x, int y, int width, int height);

    bool* tCnt;
    bool pCnt;

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE tInst;
    HWND tWnd;
    RECT toolRT;

    HWND basicBT = nullptr;
    HWND brushBT = nullptr;
    HWND pencilBT = nullptr;
    HWND sprayBT = nullptr;
    HWND markerBT = nullptr;
    HWND watercolorBT = nullptr;

    HWND Color1BT = nullptr;
    HWND Color2BT = nullptr;
    HWND Color3BT = nullptr;
    HWND PenWidthBT = nullptr;
    HWND ClearBT = nullptr;
    HWND SaveBT = nullptr;
    HWND PlayBT = nullptr;
    HWND ResetBT = nullptr;

    std::unique_ptr<Function> function;
    std::unique_ptr<ColorPalette> colorPalette;


    HRGN roundRgn;
};

#endif