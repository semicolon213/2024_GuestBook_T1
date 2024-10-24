#include "Stamp.h"

Stamp::Stamp(int size, int icon) {
    this->stampSize = size;
    this->stampIcon = icon;
    this->stampX = 0;
    this->stampY = 0;
    this->stamptext = { WIDTH_R_LEFT,
                        WIDTH_R_TOP,
                        WIDTH_R_RIGHT,
                        WIDTH_R_BOTTOM };
}

void Stamp::changeModeToPen(HWND g_Hwnd, bool* stampActive)
{
    /// ������ ��� ��Ȱ��ȭ
    *stampActive = false;

    InvalidateRect(g_Hwnd, &this->stamptext, TRUE);
    UpdateWindow(g_Hwnd);
}

void Stamp::changeModeToStamp(bool* stampActive, HWND g_Hwnd, int* stampIcon, int wParam)
{
    /// ������ ��� ��Ȱ��ȭ�Ͻ� Ȱ��ȭ
    if (!*stampActive) {
        *stampActive = true;

        InvalidateRect(g_Hwnd, &this->stamptext, TRUE);  // �ؽ�Ʈ ������ ��ȿȭ
        UpdateWindow(g_Hwnd);
    }

    /// ��ư Ŭ���� func ��
    switch (wParam)
    {
    case HEART_STAMP:
        *stampIcon = IDI_HEART_ICON;
        break;
    case UH_STAMP:
        *stampIcon = IDI_UH_ICON;
        break;
    case YUHAN_STAMP:
        *stampIcon = IDI_YUHAN_ICON;
        break;
    case YONGBIN_STAMP:
        *stampIcon = IDI_YONGBIN_ICON;
        break;
    case WHY_STAMP:
        *stampIcon = IDI_WHY_ICON;
        break;
    case GU_STAMP:
        *stampIcon = IDI_GU_ICON;
        break;
    case WHAT_STAMP:
        *stampIcon = IDI_WHAT_ICON;
        break;
    case PF_STAMP:
        *stampIcon = IDI_PF_ICON;
        break;
    case F_STAMP:
        *stampIcon = IDI_F_ICON;
        break;
    case HATE_STAMP:
        *stampIcon = IDI_HATE_ICON;
        break;
    }
}

void Stamp::handleStamp(HWND hWnd, UINT message, LPARAM lParam, std::vector<PEN_INFO>* penMemory) {

    /// DrawFunc�� ���÷��� �޼��尡 �������Ͻ� ������ ��� �Ͻ� ����
    // if (PenDraw::isReplay) { return; }

    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    HDC hdc = GetDC(hWnd);

    // ������ �ε� �� ũ�� ����
    HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(this->stampIcon), IMAGE_ICON, stampSize, stampSize, 0);

    if (!hIcon) {
        MessageBox(hWnd, L"�������� �ε��� �� �����ϴ�.", L"Error", MB_OK | MB_ICONERROR);
        ReleaseDC(hWnd, hdc);
        return;
    }

    switch (message) {

    case WM_LBUTTONDOWN:
        ///���콺 x,y ��ǥ���� �׸��� ��������
        if (HIWORD(lParam) <= PAINT_R_TOP + 45
            || HIWORD(lParam) >= PAINT_R_BOTTOM - 45
            || LOWORD(lParam) < PAINT_R_LEFT + 45
            || LOWORD(lParam) > PAINT_R_RIGHT - 45) {
            break;
        }

        //wsprintf(ShowStatus::nowStatus, L"������ ���");
        //InvalidateRect(hWnd, &ShowStatus::status_Rect, TRUE);

        stampX = x;
        stampY = y;

        // �������� �׸���
        drawIcon(hdc, stampX, stampY, hIcon);

        // �׸��� ���� ����
        PEN_INFO g_Pen_Info;

        g_Pen_Info.penCoordinate = lParam;
        g_Pen_Info.penWidth = 0;
        g_Pen_Info.penColor = 0;
        g_Pen_Info.penTime = (DWORD)GetTickCount64();
        g_Pen_Info.penState = message;

        g_Pen_Info.stampOn = true;
        g_Pen_Info.stampImg = stampIcon;
        g_Pen_Info.stampSize = stampSize;

        // �� ���� ���Ϳ� ����
        penMemory->push_back(g_Pen_Info);
        break;

    case WM_MOUSEMOVE:
        stampX = x;
        stampY = y;
        break;

    case WM_LBUTTONUP:
        // ���⼭ �߰� ����� ������ �� �ֽ��ϴ�.
        break;
    }

    ReleaseDC(hWnd, hdc);
    DestroyIcon(hIcon);
}

void Stamp::drawIcon(HDC hdc, int x, int y, HICON hIcon) {
    // �������� �߽����� �׸���
    DrawIconEx(hdc, x - stampSize / 2, y - stampSize / 2, hIcon, stampSize, stampSize, 0, NULL, DI_NORMAL);
}