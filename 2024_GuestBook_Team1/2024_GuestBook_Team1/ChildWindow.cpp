#include "ChildWindow.h"

ChildWindow::ChildWindow(HINSTANCE hInstance, COLORREF bgColor)
    : cInst(hInstance), bgColor(bgColor), cWnd(NULL), hBrush(CreateSolidBrush(bgColor)) {}

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
        0, className, windowName, WS_CHILD | WS_VISIBLE,
        x, y, width, height, hParentWnd, NULL, cInst, this);
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

LRESULT CALLBACK ChildWindow::ChildWndProc(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    ChildWindow* pThis = reinterpret_cast<ChildWindow*>(GetWindowLongPtr(cWnd, GWLP_USERDATA));

    if (message == WM_CREATE) 
    {
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = reinterpret_cast<ChildWindow*>(pCreateStruct->lpCreateParams);
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
    case WM_KILLFOCUS:
        SetForegroundWindow(cWnd); // 포커스를 잃으면 다시 자식 창으로 포커스 설정
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(cWnd, &ps);
        FillRect(hdc, &ps.rcPaint, hBrush); // 배경색으로 채우기
        EndPaint(cWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(cWnd, message, wParam, lParam);
    }
    return 0;
}
