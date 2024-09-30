#include "DW_Canvas.h"
<<<<<<< HEAD
#include "PenThickness.h"
=======
>>>>>>> origin/develop


DW_Canvas::DW_Canvas(HINSTANCE hInstance)
    :ChildWindow(RGB(255, 255, 255))
{
    cInst = hInstance;
    canvasRT = { 0 };
    canWnd = nullptr;
}

void DW_Canvas::Create(HWND hParentWnd, int x, int y, int width, int height)
{
    ChildWindow::Create(hParentWnd, L"DW_CanvasClass", L"Canvas Child Window", x, y, width, height);
    canWnd = cWnd;

    Function::hWnd = canWnd;
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
        function->GDIPlusStart(); // 붓 gdi 라이브러리 활성화
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
        hdc = GetDC(canWnd);

        drawPInfo.lParam = lParam;
        drawPInfo.pColor = ColorPalette::colorArr[Function::penNum];
        drawPInfo.pTime = (DWORD)GetTickCount64();
<<<<<<< HEAD
        drawPInfo.pWidth = PenThickness::GetPenWidth(); /// 펜 굵기 설정
=======
        drawPInfo.pWidth = 20;
>>>>>>> origin/develop
        drawPInfo.state = message;
        function->draw(cWnd, drawPInfo, TRUE); // 브러쉬 기능 추가하려면 해당 RECTANGLE 에 알맞는 변수를 넣으면 됨.
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        if (function->getIsReplay()) break;
        drawPInfo.lParam = lParam;
        drawPInfo.pColor = ColorPalette::colorArr[Function::penNum];
        drawPInfo.pTime = (DWORD)GetTickCount64();
<<<<<<< HEAD
        drawPInfo.pWidth = PenThickness::GetPenWidth(); /// 펜 굵기 설정
=======
        drawPInfo.pWidth = 20;
>>>>>>> origin/develop
        drawPInfo.state = message;
        function->mouseUD(drawPInfo, TRUE);

        break;

    case WM_RBUTTONDOWN:

        break;

    case WM_SIZE:


    case WM_ERASEBKGND:
        ///화면 깜빡임 방지
        return DefWindowProc(cWnd, message, wParam, lParam);
        break;
    case WM_PAINT:
        canvasRT = ChildWindow::GetRT();
        CHdc = GetDC(canWnd);
        CHdc = BeginPaint(canWnd, &C_ps);
        CanvasPen = (HPEN)SelectObject(CHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
        Rectangle(CHdc, canvasRT.left, canvasRT.top, canvasRT.right, canvasRT.bottom);
        SelectObject(CHdc, CanvasPen);
        DeleteObject(CanvasPen);
        EndPaint(canWnd, &C_ps);

    default:
        return ChildWindow::HandleMessage(canWnd, message, wParam, lParam);
    }
    return 0;
}