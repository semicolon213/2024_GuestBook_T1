#include "DrowWindow.h"

DrowWindow::DrowWindow(int mode, HINSTANCE hInst)
    : mode(mode), hInst(hInst), hwnd(nullptr), bkColor(RGB(249, 243, 240)) {
    wndFunc = std::make_unique<WndFunc>();
    this->wc = {};
}

/// ���ӹ� ���� �޼���
void DrowWindow::createWindowNB(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc10 = {};
    wc10.lpfnWndProc = WndProcNB;  // ���ӹ� �޼��� ó���ϴ� ���� �޼���
    wc10.lpszClassName = L"CustomNameWindowClass";
    wc10.hInstance = hInst;
    wc10.hbrBackground = CreateSolidBrush(RGB(243, 243, 243));


    if (!RegisterClass(&wc10)) {
        MessageBox(NULL, L"���� �� ��� ����", L"Error", MB_OK);
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
        reinterpret_cast<LPVOID>(this)  // this ������ ����
    );
    if (!WndFunc::nameWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"���� �� ���� ����: ���� �ڵ� %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }

    ShowWindow(WndFunc::nameWnd, SW_SHOW);
}

/// ���� ���� �޼���
void DrowWindow::createWindowTB(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc3 = {};
    wc3.lpfnWndProc = WndProcTB;  // ���ӹ� �޼��� ó���ϴ� ���� �޼���
    wc3.lpszClassName = L"Tool";
    wc3.hInstance = hInst;
    wc3.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));


    if (!RegisterClass(&wc3)) {
        MessageBox(NULL, L"���� ��� ����", L"Error", MB_OK);
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
        reinterpret_cast<LPVOID>(this)  // this ������ ����
    );
    if (!WndFunc::toolWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"���� ���� ����: ���� �ڵ� %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::toolWnd, SW_SHOW);
}

/// ĵ���� ���� �޼���
void DrowWindow::createWindowCV(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc2 = {};
    wc2.lpfnWndProc = WndProcCV;  // ���ӹ� �޼��� ó���ϴ� ���� �޼���
    wc2.lpszClassName = L"CustomNameWindowClass1";
    wc2.hInstance = hInst;
    wc2.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));


    if (!RegisterClass(&wc2)) {
        MessageBox(NULL, L"cavas ��� ����", L"Error", MB_OK);
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
        reinterpret_cast<LPVOID>(this)  // this ������ ����
    );
    if (!WndFunc::canvasWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"���� �� ���� ����: ���� �ڵ� %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::canvasWnd, SW_SHOW);
}

/// �湮�� ����Ʈ ���� �޼���
void DrowWindow::createWindowVL(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc11 = {};
    wc11.lpfnWndProc = WndProcVL;  // ���ӹ� �޼��� ó���ϴ� ���� �޼���
    wc11.lpszClassName = L"CustomNameWindowClass3";
    wc11.hInstance = hInst;
    wc11.hbrBackground = CreateSolidBrush(RGB(249, 243, 240));


    if (!RegisterClass(&wc11)) {
        MessageBox(NULL, L"�湮�� ����Ʈ ��� ����", L"Error", MB_OK);
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
        reinterpret_cast<LPVOID>(this)  // this ������ ����
    );
    if (!WndFunc::visitListWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"�湮�ڸ���Ʈ ������ ���� ����: ���� �ڵ� %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    HFONT hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, TEXT("�������"));

    // STATIC ��Ʈ�ѿ� ��Ʈ ����
    SendMessage(WndFunc::visitListWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

    ShowWindow(WndFunc::visitListWnd, SW_SHOW);
}
    /// ���� ���� �޼���
void DrowWindow::createWindowCP(int left, int top, int right, int bottom, HWND parent)
{
    WNDCLASS wc31 = {};
    wc31.lpfnWndProc = WndProcCP;  // ���ӹ� �޼��� ó���ϴ� ���� �޼���
    wc31.lpszClassName = L"Tototo";
    wc31.hInstance = hInst;
    wc31.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));


    if (!RegisterClass(&wc31)) {
        MessageBox(NULL, L"�÷��ڽ� ��� ����", L"Error", MB_OK);
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
        reinterpret_cast<LPVOID>(this)  // this ������ ����
    );
    if (!WndFunc::colorWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"���� ���� ����: ���� �ڵ� %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::colorWnd, SW_SHOW);
}

// ���� ������ ���ν���
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
        return pThis->handleMessage(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/// ���� �Լ��� �޽��� ó��
LRESULT DrowWindow::handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// ���ӹ� ������ ����
        createWindowNB(0, 0, WndFunc::wndSize.right, 57, hWnd);

        /// ���� ������ ����
        createWindowTB(-1, 57, WndFunc::wndSize.right, 51, hWnd);

        /// ĵ���� ������ ����
        createWindowCV((WndFunc::wndSize.right - 1300) / 2, (WndFunc::wndSize.bottom - 600) / 2, 1300, 700, hWnd);

        /// ������ ������ ����
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