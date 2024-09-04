#pragma once
#include "ChildWindow.h"

class BlueWindow : public ChildWindow {
public:
    BlueWindow(HINSTANCE bInstance);
    void Create(HWND hParentWnd, int x, int y, int width, int height);

protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE bInst;
    HWND bWnd;
};