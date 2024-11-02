#include "DW_NameBar.h"

/// 사이드바 생성 메서드
void DrowWindow::createWindowSB(int left, int top, int right, int bottom, HWND parent)
{
    /// 윈도우 등록은 한번만 해야하기 때문에 윈도우 등록 코드는 한번만 실행

    static bool isClassRegistered = false;  /// 클래스가 이미 등록되었는지 확인

    if (!isClassRegistered) {
        WNDCLASS wc5 = {};
        wc5.lpfnWndProc = WndProcSB;  /// 네임바 메세지 처리하는 정적 메서드
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
MakeButton exitButton(10, 10, 40, 40);
RECT mousePoint;
RECT a1;

/// 네임 바 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SETTEXT:
        /// WM_SETTEXT 메시지 처리
        /// save나 로드시 namebar 텍스트 변경
        SetWindowText(fileNameW, reinterpret_cast<LPCWSTR>(lParam));
        break;
    case WM_CREATE:
    {
        /*
        backB = CreateWindow(L"BUTTON", L"<", WS_CHILD | WS_VISIBLE,
            10, 12, 30, 30, hWnd, (HMENU)NB_BACK_BT, nullptr, NULL);
*/
        fileNameW = CreateWindow(L"STATIC", L"이름 없음", WS_CHILD | WS_VISIBLE,
            50, 12, 300, 30, hWnd, (HMENU)NB_FILE_NAME, nullptr, NULL);

        sideMenu.setCoordinate(WndFunc::wndSize.right - 40, 10, WndFunc::wndSize.right - 10, 40);

        HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, TEXT("나눔고딕"));

        // STATIC 컨트롤에 폰트 설정
        SendMessage(fileNameW, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;
    }
    case WM_SIZE:
    {
        /// 창 크기 변화시 사이드 메뉴 버튼 이동
        sideMenu.setCoordinate(WndFunc::wndSize.right - 40, 10, WndFunc::wndSize.right - 10, 40);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        /// 버튼 클릭 확인용 마우스 좌표 기준 RECT 생성
        mousePoint.left = LOWORD(lParam);
        mousePoint.top = HIWORD(lParam);
        mousePoint.right = mousePoint.left + 1;
        mousePoint.bottom = mousePoint.top + 1;

        /// 사이드 윈도우 존재 시 창 삭제
        if (IntersectRect(&a1, &mousePoint, &sideMenu.rectButton)) {
            /// 토글 상태 반전(버튼 이미지 변경)
            sideMenu.toggleState = !sideMenu.toggleState;

            /// 현재 사이드바가 열려있지 않을때 실행
            if (WndFunc::sideWnd == nullptr) {
                createWindowSB(WndFunc::wndSize.right - 60, 110, 60, 300, WndFunc::drowWnd);
            }
            else {
                /// 파일매니저와 리스트 박스 끄기
                ShowWindow(DW_FileManager::hListBox, SW_HIDE); 
                ShowWindow(WndFunc::fileManager, SW_HIDE);
                /// 사이드 윈도우 DestroyWindow
                DestroyWindow(WndFunc::sideWnd);
                /// 사이드 윈도우 핸들값 초기화
                WndFunc::sideWnd = nullptr;
            }
        }

        if (IntersectRect(&a1, &mousePoint, &exitButton.rectButton)) {
            WndFunc::buttonOn = true;

            ShowWindow(WndFunc::drowWnd, SW_HIDE);
            ShowWindow(WndFunc::nameWnd, SW_HIDE);
            ShowWindow(WndFunc::toolWnd, SW_HIDE);
            ShowWindow(WndFunc::canvasWnd, SW_HIDE);
            ShowWindow(WndFunc::sideWnd, SW_HIDE);
            ShowWindow(WndFunc::visitListWnd, SW_HIDE);
            
            ShowWindow(WndFunc::DrowBT, SW_SHOW);
            ShowWindow(WndFunc::LoadBT, SW_SHOW);
            ShowWindow(WndFunc::CreditBT, SW_SHOW);
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_COMMAND:
    {
        int wmId = wParam;

        switch (wmId)
        {

        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        /// 사이드 버튼의 이미지 버튼 두개 
        sideMenu.doubleImgButton(hdc, IDI_CLOSE_MENU_ICON, IDI_MENU_ICON);
        exitButton.drawRectButton(hdc, IDI_EXIT_ICON);
        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


