#include "ChildWindow.h"

ChildWindow::ChildWindow(HINSTANCE hInstance, COLORREF bgColor)
    : cInst(hInstance), bgColor(bgColor), cWnd(NULL), hBrush(CreateSolidBrush(bgColor))
{
    ChildRT = { 0 };
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
    //case WM_KILLFOCUS:
    //    SetForegroundWindow(cWnd); // ��Ŀ���� ������ �ٽ� �ڽ� â���� ��Ŀ�� ����
    //    break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(cWnd, &ps);
        FillRect(hdc, &ps.rcPaint, hBrush); // �������� ä���
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
