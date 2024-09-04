#pragma once
#include <windows.h>
#include "Resource.h"

class ChildWindow 
{
public:
    ChildWindow(HINSTANCE hInstance, COLORREF bgColor);
    virtual ~ChildWindow() = default;
    virtual void Create(HWND hParentWnd, LPCWSTR className, LPCWSTR windowName, int x, int y, int width, int height);
    RECT GetRT();
    void Show(bool show);
    HWND GetHWND() const;

protected:
    virtual LRESULT HandleMessage(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE cInst;
    COLORREF bgColor;
    HWND cWnd;
    HBRUSH hBrush;
    RECT ChildRT;

private:
    static LRESULT CALLBACK ChildWndProc(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam);
};