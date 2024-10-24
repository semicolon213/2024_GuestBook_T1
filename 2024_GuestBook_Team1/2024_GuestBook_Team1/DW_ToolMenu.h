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

    std::unique_ptr<Function> function;
    std::unique_ptr<ColorPalette> colorPalette;

    HRGN roundRgn;
};

#endif