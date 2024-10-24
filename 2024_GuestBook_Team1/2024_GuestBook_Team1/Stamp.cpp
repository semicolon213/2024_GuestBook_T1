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
    /// 스탬프 모드 비활성화
    *stampActive = false;

    InvalidateRect(g_Hwnd, &this->stamptext, TRUE);
    UpdateWindow(g_Hwnd);
}

void Stamp::changeModeToStamp(bool* stampActive, HWND g_Hwnd, int* stampIcon, int wParam)
{
    /// 스탬프 모드 비활성화일시 활성화
    if (!*stampActive) {
        *stampActive = true;

        InvalidateRect(g_Hwnd, &this->stamptext, TRUE);  // 텍스트 영역만 무효화
        UpdateWindow(g_Hwnd);
    }

    /// 버튼 클릭한 func 값
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

    /// DrawFunc의 리플레이 메서드가 실행중일시 스탬프 기능 일시 정지
    // if (PenDraw::isReplay) { return; }

    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    HDC hdc = GetDC(hWnd);

    // 아이콘 로드 및 크기 조정
    HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(this->stampIcon), IMAGE_ICON, stampSize, stampSize, 0);

    if (!hIcon) {
        MessageBox(hWnd, L"아이콘을 로드할 수 없습니다.", L"Error", MB_OK | MB_ICONERROR);
        ReleaseDC(hWnd, hdc);
        return;
    }

    switch (message) {

    case WM_LBUTTONDOWN:
        ///마우스 x,y 좌표기준 그리기 영역지정
        if (HIWORD(lParam) <= PAINT_R_TOP + 45
            || HIWORD(lParam) >= PAINT_R_BOTTOM - 45
            || LOWORD(lParam) < PAINT_R_LEFT + 45
            || LOWORD(lParam) > PAINT_R_RIGHT - 45) {
            break;
        }

        //wsprintf(ShowStatus::nowStatus, L"스탬프 모드");
        //InvalidateRect(hWnd, &ShowStatus::status_Rect, TRUE);

        stampX = x;
        stampY = y;

        // 아이콘을 그리기
        drawIcon(hdc, stampX, stampY, hIcon);

        // 그리기 정보 저장
        PEN_INFO g_Pen_Info;

        g_Pen_Info.penCoordinate = lParam;
        g_Pen_Info.penWidth = 0;
        g_Pen_Info.penColor = 0;
        g_Pen_Info.penTime = (DWORD)GetTickCount64();
        g_Pen_Info.penState = message;

        g_Pen_Info.stampOn = true;
        g_Pen_Info.stampImg = stampIcon;
        g_Pen_Info.stampSize = stampSize;

        // 펜 정보 벡터에 저장
        penMemory->push_back(g_Pen_Info);
        break;

    case WM_MOUSEMOVE:
        stampX = x;
        stampY = y;
        break;

    case WM_LBUTTONUP:
        // 여기서 추가 기능을 구현할 수 있습니다.
        break;
    }

    ReleaseDC(hWnd, hdc);
    DestroyIcon(hIcon);
}

void Stamp::drawIcon(HDC hdc, int x, int y, HICON hIcon) {
    // 아이콘을 중심으로 그리기
    DrawIconEx(hdc, x - stampSize / 2, y - stampSize / 2, hIcon, stampSize, stampSize, 0, NULL, DI_NORMAL);
}