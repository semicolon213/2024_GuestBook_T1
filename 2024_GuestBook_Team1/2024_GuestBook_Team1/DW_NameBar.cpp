#include "DW_NameBar.h"

void DrowWindow::createWindowSB(int left, int top, int right, int bottom, HWND parent)
{
    static bool isClassRegistered = false;  // 클래스가 이미 등록되었는지 확인

    if (!isClassRegistered) {
        WNDCLASS wc5 = {};
        wc5.lpfnWndProc = WndProcSB;  // 네임바 메세지 처리하는 정적 메서드
        wc5.lpszClassName = L"CustomNameWindowClass2";
        wc5.hInstance = hInst;
        wc5.hbrBackground = CreateSolidBrush(RGB(230, 230, 230));


        if (!RegisterClass(&wc5)) {
            MessageBox(NULL, L"side 바 등록 실패", L"Error", MB_OK);
            return;
        }

        isClassRegistered = true;  // 클래스가 등록됨을 표시
    }
    

    WndFunc::sideWnd = CreateWindow(
        L"CustomNameWindowClass2",
        L"Name Window",
        WS_CHILD | WS_VISIBLE,
        left, top,
        right,
        bottom,
        parent,
        nullptr,
        hInst,
        reinterpret_cast<LPVOID>(this)  // this 포인터 전달
    );
    if (!WndFunc::sideWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"사이드 바 생성 실패: 오류 코드 %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::sideWnd, SW_SHOW);
}


/// 네임 바 정적 메서드
LRESULT CALLBACK DrowWindow::WndProcNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
   
    DrowWindow* pThis = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<DrowWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<DrowWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->handleMessageNB(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND backB = nullptr;
HWND fileNameW = nullptr;
HWND sideB = nullptr;

MakeButton sideMenu;
RECT mousePoint;
RECT a1;

/// 네임 바 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) 
    {
    case WM_CREATE:
    {
        backB = CreateWindow(L"BUTTON", L"<", WS_CHILD | WS_VISIBLE,
            10, 12, 30, 30, hWnd, (HMENU)NB_BACK_BT, nullptr, NULL);

        fileNameW = CreateWindow(L"STATIC", L"이름 없음", WS_CHILD | WS_VISIBLE,
            50, 12, 300, 30, hWnd, (HMENU)NB_FILE_NAME, nullptr, NULL);

        sideMenu.setCoordinate(WndFunc::wndSize.right - 40, 10, WndFunc::wndSize.right - 10, 40);
        /*
        sideB = CreateWindow(L"BUTTON", L":", WS_CHILD | WS_VISIBLE,
           WndFunc::wndSize.right - 40 , 12, 30, 30, hWnd, (HMENU)NB_SIDE_BT, nullptr, NULL);
           */
        HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, TEXT("나눔고딕"));

        // STATIC 컨트롤에 폰트 설정
        SendMessage(fileNameW, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        mousePoint.left = LOWORD(lParam);
        mousePoint.top = HIWORD(lParam);
        mousePoint.right = mousePoint.left + 1;
        mousePoint.bottom = mousePoint.top + 1;

        // 사이드 윈도우 존재 시 창 삭제
        if (IntersectRect(&a1, &mousePoint, &sideMenu.rectButton)) {
            sideMenu.toggleState = !sideMenu.toggleState;  // 토글 상태 반전

            if (WndFunc::sideWnd == nullptr) {  // 사이드바가 없을 때만 생성
               
                createWindowSB(WndFunc::wndSize.right - 60, 110, 60, 300, WndFunc::drowWnd);
            }
            else {  // 이미 열려 있으면 창 삭제
                DestroyWindow(WndFunc::sideWnd);
                WndFunc::sideWnd = nullptr;
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {

        }
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(WndFunc::nameWnd, &ps);

        sideMenu.doubleImgButton(hdc, IDI_CLOSE_MENU_ICON, IDI_MENU_ICON);

        EndPaint(WndFunc::nameWnd, &ps);
        break;
    }
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


