#include "ChildWindow.h"

ChildWindow::ChildWindow(COLORREF bgColor)
    : cInst(nullptr), bgColor(bgColor), cWnd(NULL), hBrush(CreateSolidBrush(bgColor))
{
    ChildRT = { 0 };
}

void ChildWindow::CreatePop(HWND hParentWnd, LPCWSTR className, LPCWSTR windowName, int x, int y, int width, int height) 
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = ChildWndProc;
    wc.hInstance = cInst;
    wc.lpszClassName = className;
    wc.hbrBackground = hBrush;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClass(&wc);

    cWnd = CreateWindowEx(
        0, className, windowName, WS_POPUP | WS_VISIBLE,
        x, y, width, height, hParentWnd, NULL, cInst, this);

    cInst = (HINSTANCE)GetWindowLongPtr(cWnd, GWLP_HINSTANCE);
}

void ChildWindow::Create(HWND hParentWnd, LPCWSTR className, LPCWSTR windowName, int x, int y, int width, int height)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = ChildWndProc;
    wc.hInstance = cInst;
    wc.lpszClassName = className;
    wc.hbrBackground = hBrush;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClass(&wc);

    cWnd = CreateWindowEx(
        0, className, windowName, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
        x, y, width, height, hParentWnd, NULL, cInst, this);

    cInst = (HINSTANCE)GetWindowLongPtr(cWnd, GWLP_HINSTANCE);
}

void ChildWindow::Show(bool show) 
{
    ShowWindow(cWnd, show ? SW_SHOW : SW_HIDE);
}

RECT ChildWindow::GetRT()
{
    GetClientRect(cWnd, &ChildRT);
    return ChildRT;
}

HWND ChildWindow::GetHWND() const 
{
    return cWnd;
}

RECT ChildWindow::GetChildPos(HWND hWndParent, HWND hWndChild)
{
    RECT rect;
    GetClientRect(hWndChild, &rect);
    MapWindowPoints(hWndChild, hWndParent, (POINT*)&rect, 2);
    
    return rect;
}

LRESULT CALLBACK ChildWindow::ChildWndProc(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    ChildWindow* pThis = (ChildWindow*)GetWindowLongPtr(cWnd, GWLP_USERDATA);

    if (message == WM_CREATE) 
    {
        LPCREATESTRUCT pCreateStruct = (LPCREATESTRUCT)lParam;
        pThis = (ChildWindow*)pCreateStruct->lpCreateParams;
        SetWindowLongPtr(cWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }

    if (pThis) 
    {
        return pThis->HandleMessage(cWnd, message, wParam, lParam);
    }
    else 
    {
        return DefWindowProc(cWnd, message, wParam, lParam);
    }
}

LRESULT ChildWindow::HandleMessage(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message) 
    {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(cWnd, message, wParam, lParam);
    }
    return 0;
}
