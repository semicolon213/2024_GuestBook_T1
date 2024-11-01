#include "DrowWindow.h"

DrowWindow::DrowWindow(int mode, HINSTANCE hInst)
    : mode(mode), hInst(hInst), hwnd(nullptr), bkColor(RGB(249, 243, 240)) {
    wndFunc = std::make_unique<WndFunc>();
    this->wc = {};
}

/// 네임바 생성 메서드
void DrowWindow::createWindowNB(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc10 = {};
    wc10.lpfnWndProc = WndProcNB;  // 네임바 메세지 처리하는 정적 메서드
    wc10.lpszClassName = L"CustomNameWindowClass";
    wc10.hInstance = hInst;
    wc10.hbrBackground = CreateSolidBrush(RGB(243, 243, 243));


    if (!RegisterClass(&wc10)) {
        MessageBox(NULL, L"네임 바 등록 실패", L"Error", MB_OK);
        return;
    }
    WndFunc::nameWnd = CreateWindow(
        L"CustomNameWindowClass",
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
    if (!WndFunc::nameWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"네임 바 생성 실패: 오류 코드 %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }

    ShowWindow(WndFunc::nameWnd, SW_SHOW);
}

/// 툴바 생성 메서드
void DrowWindow::createWindowTB(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc3 = {};
    wc3.lpfnWndProc = WndProcTB;  // 네임바 메세지 처리하는 정적 메서드
    wc3.lpszClassName = L"Tool";
    wc3.hInstance = hInst;
    wc3.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));


    if (!RegisterClass(&wc3)) {
        MessageBox(NULL, L"툴바 등록 실패", L"Error", MB_OK);
        return;
    }
    WndFunc::toolWnd = CreateWindow(
        L"Tool",
        L"Tool1",
        WS_CHILD | WS_VISIBLE,
        left, top,
        right,
        bottom,
        parent,
        nullptr,
        hInst,
        reinterpret_cast<LPVOID>(this)  // this 포인터 전달
    );
    if (!WndFunc::toolWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"툴바 생성 실패: 오류 코드 %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::toolWnd, SW_SHOW);
}

/// 캔버스 생성 메서드
void DrowWindow::createWindowCV(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc2 = {};
    wc2.lpfnWndProc = WndProcCV;  // 네임바 메세지 처리하는 정적 메서드
    wc2.lpszClassName = L"CustomNameWindowClass1";
    wc2.hInstance = hInst;
    wc2.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));


    if (!RegisterClass(&wc2)) {
        MessageBox(NULL, L"cavas 등록 실패", L"Error", MB_OK);
        return;
    }
    WndFunc::canvasWnd = CreateWindow(
        L"CustomNameWindowClass1",
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
    if (!WndFunc::canvasWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"네임 바 생성 실패: 오류 코드 %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::canvasWnd, SW_SHOW);
}

/// 방문자 리스트 생성 메서드
void DrowWindow::createWindowVL(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc11 = {};
    wc11.lpfnWndProc = WndProcVL;  // 네임바 메세지 처리하는 정적 메서드
    wc11.lpszClassName = L"CustomNameWindowClass3";
    wc11.hInstance = hInst;
    wc11.hbrBackground = CreateSolidBrush(RGB(249, 243, 240));


    if (!RegisterClass(&wc11)) {
        MessageBox(NULL, L"방문자 리스트 등록 실패", L"Error", MB_OK);
        return;
    }
    WndFunc::visitListWnd = CreateWindow(
        L"CustomNameWindowClass3",
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
    if (!WndFunc::visitListWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"방문자리스트 윈도우 생성 실패: 오류 코드 %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    HFONT hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, TEXT("나눔고딕"));

    // STATIC 컨트롤에 폰트 설정
    SendMessage(WndFunc::visitListWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

    ShowWindow(WndFunc::visitListWnd, SW_SHOW);
}
    /// 툴바 생성 메서드
void DrowWindow::createWindowCP(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc31 = {};
    wc31.lpfnWndProc = WndProcCP;  // 네임바 메세지 처리하는 정적 메서드
    wc31.lpszClassName = L"Tototo";
    wc31.hInstance = hInst;
    wc31.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));


    if (!RegisterClass(&wc31)) {
        MessageBox(NULL, L"컬러박스 등록 실패", L"Error", MB_OK);
        return;
    }
    WndFunc::colorWnd = CreateWindow(
        L"Tototo",
        L"Tototo",
        WS_CHILD | WS_VISIBLE,
        left, top,
        right,
        bottom,
        parent,
        nullptr,
        hInst,
        reinterpret_cast<LPVOID>(this)  // this 포인터 전달
    );
    if (!WndFunc::colorWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"툴바 생성 실패: 오류 코드 %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::colorWnd, SW_SHOW);
}

// 정적 윈도우 프로시저
LRESULT CALLBACK DrowWindow::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
        return pThis->handleMessage(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/// 가상 함수로 메시지 처리
LRESULT DrowWindow::handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// 네임바 윈도우 생성
        createWindowNB(0, 0, WndFunc::wndSize.right, 57, hWnd);

        /// 툴바 윈도우 생성
        createWindowTB(-1, 57, WndFunc::wndSize.right, 51, hWnd);

        /// 캔버스 윈도우 생성
        createWindowCV((WndFunc::wndSize.right - 1300) / 2, (WndFunc::wndSize.bottom - 600) / 2, 1300, 700, hWnd);

        /// 전광판 윈도우 생성
        createWindowVL(0, WndFunc::wndSize.bottom - 30, WndFunc::wndSize.right, WndFunc::wndSize.bottom, hWnd);

        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}