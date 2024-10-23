#include "DW_Canvas.h"
#include "PenThickness.h"
#include "ColorPalette.h"


DW_Canvas::DW_Canvas(HINSTANCE hInstance)
    : ChildWindow(RGB(255, 255, 255))
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
    penThickness = make_unique<PenThickness>();
}

LRESULT DW_Canvas::HandleMessage(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
        function = make_unique<Function>();
        function->GDIPlusStart(); // 붓 gdi 라이브러리 활성화
        break;

    case WM_COMMAND:
        // 색상 및 굵기 버튼 클릭 처리
        if (LOWORD(wParam) == ID_COLOR_THICKNESS_BUTTON) // 버튼 ID에 따라 수정
        {
            // 색상 선택 기능 호출
            ColorPalette colorPalette;
            colorPalette.colorSelect(canWnd, 0); // 첫 번째 펜에 대한 색상 선택

            // 펜 굵기 설정 대화 상자 호출
            if (DialogBoxParam(cInst, MAKEINTRESOURCE(IDD_PEN_THICKNESS_DIALOG), canWnd, PenThickness::dlgProc, 0) == IDOK) {
                // 선택된 펜 굵기 값 가져오기
                int thickness = penThickness->getPenWidth();

                // 선택된 색상과 굵기 출력 (디버깅용, 원하는 대로 처리)
                COLORREF selectedColor = colorPalette.getColor(0);
                // 예: printf("선택된 색상: %x, 선택된 굵기: %d\n", selectedColor, thickness);

                // 선택된 펜 굵기와 색상을 적용하는 로직 추가
                // 예: SetPenThicknessAndColor(thickness, selectedColor);
            }
        }

        if (wParam == TL_CLEAR_BT)
        {
            if (!function->getIsReplay())
            {
                function->clearDrawing(cWnd);
            }
            break;
        }

        if (wParam == TL_PLAY_BT)
        {
            function->setIsReset(false);
            function->replayThread(cWnd);
        }

        if (wParam == TL_PLAY_BT && lParam == 1)
        {
            function->setIsReplay(false);
        }

        if (!function->getIsReset())
        {
            if (wParam == TL_RESET_BT)
            {
                function->stopReplay(cWnd);
            }
            if (wParam == TL_RESET_BT && lParam == 1)
            {
                function->reDrawing(cWnd);
            }
        }
        break;

    case WM_MOUSEMOVE:
        if (function->getIsReplay()) break;
        hdc = GetDC(canWnd);

        drawPInfo.lParam = lParam;
        drawPInfo.pColor = ColorPalette::colorArr[Function::penNum];
        drawPInfo.pTime = (DWORD)GetTickCount64();
        drawPInfo.pWidth = penThickness->getPenWidth(); /// 펜 굵기 설정
        drawPInfo.state = message;
        function->draw(cWnd, drawPInfo, TRUE); // 브러쉬 기능 추가하려면 해당 RECTANGLE 에 알맞는 변수를 넣으면 됨.
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        if (function->getIsReplay()) break;
        drawPInfo.lParam = lParam;
        drawPInfo.pColor = ColorPalette::colorArr[Function::penNum];
        drawPInfo.pTime = (DWORD)GetTickCount64();
        drawPInfo.pWidth = penThickness->getPenWidth(); /// 펜 굵기 설정
        drawPInfo.state = message;
        function->mouseUD(drawPInfo, TRUE);
        break;

    case WM_RBUTTONDOWN:
        break;

    case WM_SIZE:
        break;

    case WM_ERASEBKGND:
        // 화면 깜빡임 방지
        return DefWindowProc(cWnd, message, wParam, lParam);
        break;

    case WM_PAINT:
        canvasRT = ChildWindow::GetRT();
        function->paint(canWnd, canvasRT);
        break;

    default:
        return ChildWindow::HandleMessage(cWnd, message, wParam, lParam);
    }
    return 0;
}
