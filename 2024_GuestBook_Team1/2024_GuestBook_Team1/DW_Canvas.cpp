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
        function->GDIPlusStart(); // �� gdi ���̺귯�� Ȱ��ȭ
        break;

    case WM_COMMAND:
        // ���� �� ���� ��ư Ŭ�� ó��
        if (LOWORD(wParam) == ID_COLOR_THICKNESS_BUTTON) // ��ư ID�� ���� ����
        {
            // ���� ���� ��� ȣ��
            ColorPalette colorPalette;
            colorPalette.colorSelect(canWnd, 0); // ù ��° �濡 ���� ���� ����

            // �� ���� ���� ��ȭ ���� ȣ��
            if (DialogBoxParam(cInst, MAKEINTRESOURCE(IDD_PEN_THICKNESS_DIALOG), canWnd, PenThickness::dlgProc, 0) == IDOK) {
                // ���õ� �� ���� �� ��������
                int thickness = penThickness->getPenWidth();

                // ���õ� ����� ���� ��� (������, ���ϴ� ��� ó��)
                COLORREF selectedColor = colorPalette.getColor(0);
                // ��: printf("���õ� ����: %x, ���õ� ����: %d\n", selectedColor, thickness);

                // ���õ� �� ����� ������ �����ϴ� ���� �߰�
                // ��: SetPenThicknessAndColor(thickness, selectedColor);
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
        drawPInfo.pWidth = penThickness->getPenWidth(); /// �� ���� ����
        drawPInfo.state = message;
        function->draw(cWnd, drawPInfo, TRUE); // �귯�� ��� �߰��Ϸ��� �ش� RECTANGLE �� �˸´� ������ ������ ��.
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        if (function->getIsReplay()) break;
        drawPInfo.lParam = lParam;
        drawPInfo.pColor = ColorPalette::colorArr[Function::penNum];
        drawPInfo.pTime = (DWORD)GetTickCount64();
        drawPInfo.pWidth = penThickness->getPenWidth(); /// �� ���� ����
        drawPInfo.state = message;
        function->mouseUD(drawPInfo, TRUE);
        break;

    case WM_RBUTTONDOWN:
        break;

    case WM_SIZE:
        break;

    case WM_ERASEBKGND:
        // ȭ�� ������ ����
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
