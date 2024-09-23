#include "DW_Canvas.h"
DW_Canvas::DW_Canvas(HINSTANCE hInstance)
    :ChildWindow(RGB(255, 255, 255))
{
    CInst = hInstance;
    CanvasRT = { 0 };
    CWnd = nullptr;
}

void DW_Canvas::Create(HWND hParentWnd, int x, int y, int width, int height)
{
    ChildWindow::Create(hParentWnd, L"DW_CanvasClass", L"Canvas Child Window", x, y, width, height);
    CWnd = cWnd;
}

PAINTSTRUCT C_ps = { 0 };
HBRUSH CanvasBrush = nullptr;
HPEN CanvasPen = nullptr;
HDC CHdc = nullptr;

LRESULT DW_Canvas::HandleMessage(HWND tWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        CanvasRT = ChildWindow::GetRT();
    case WM_CREATE:


        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {


        }
        break;
    case WM_PAINT:
        CanvasRT = ChildWindow::GetRT();
        CHdc = GetDC(tWnd);
        CHdc = BeginPaint(tWnd, &C_ps);
        CanvasBrush = (HBRUSH)SelectObject(CHdc, GetStockObject(NULL_BRUSH));
        CanvasPen = (HPEN)SelectObject(CHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
        Rectangle(CHdc, CanvasRT.left, CanvasRT.top, CanvasRT.right, CanvasRT.bottom);
        SelectObject(CHdc, CanvasPen);
        SelectObject(CHdc, CanvasBrush);
        EndPaint(tWnd, &C_ps);

    default:
        return ChildWindow::HandleMessage(tWnd, message, wParam, lParam);
    }
    return 0;
}