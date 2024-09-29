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

    Function::hWnd = CWnd;        
}

PAINTSTRUCT C_ps = { 0 };
HBRUSH CanvasBrush = nullptr;
HPEN CanvasPen = nullptr;
HDC CHdc = nullptr;

LRESULT DW_Canvas::HandleMessage(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
        function = make_unique<Function>();

        break;

    case WM_COMMAND:
        if (wParam == TL_CLEAR_BT)
        {
            function->clearDrawing(cWnd);
            break;
        }
        if (wParam == TL_PLAY_BT)
        {
            function->replayThread(cWnd);
        }
        if (wParam == TL_PLAY_BT && lParam == 1)
        {
            function->setIsReplay(false);
        }
        if (wParam == TL_RESET_BT)
        {
            function->stopReplay(cWnd);
        }
        if (wParam == TL_RESET_BT && lParam == 1)
        {
            function->reDrawing(cWnd);
        }
        if (LOWORD(wParam) == 1) 
        {
        }
        break;

    case WM_MOUSEMOVE:
        if (function->getIsReplay()) break;
        hdc = GetDC(CWnd);

        drawPInfo.lParam = lParam;
        drawPInfo.pColor = colorPalette->getColor(penNum);
        drawPInfo.pTime = (DWORD)GetTickCount64();
        drawPInfo.pWidth = 10;
        drawPInfo.state = message;
        function->draw(cWnd, drawPInfo, TRUE); // �귯�� ��� �߰��Ϸ��� �ش� RECTANGLE �� �˸´� ������ ������ ��.
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        if (function->getIsReplay()) break;
        drawPInfo.lParam = lParam;
        drawPInfo.pColor = colorPalette->getColor(penNum);
        drawPInfo.pTime = (DWORD)GetTickCount64();
        drawPInfo.pWidth = 10;
        drawPInfo.state = message;
        function->mouseUD(drawPInfo, TRUE);

        break;

    case WM_RBUTTONDOWN:

        break;

    case WM_SIZE:


    case WM_ERASEBKGND:
        ///ȭ�� ������ ����
        return DefWindowProc(cWnd, message, wParam, lParam);
        break;
    case WM_PAINT:
        CanvasRT = ChildWindow::GetRT();
        CHdc = GetDC(CWnd);
        CHdc = BeginPaint(CWnd, &C_ps);
        CanvasBrush = (HBRUSH)SelectObject(CHdc, GetStockObject(NULL_BRUSH));
        CanvasPen = (HPEN)SelectObject(CHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
        Rectangle(CHdc, CanvasRT.left, CanvasRT.top, CanvasRT.right, CanvasRT.bottom);
        SelectObject(CHdc, CanvasPen);
        SelectObject(CHdc, CanvasBrush);
        EndPaint(CWnd, &C_ps);

    default:
        return ChildWindow::HandleMessage(CWnd, message, wParam, lParam);
    }
    return 0;
}