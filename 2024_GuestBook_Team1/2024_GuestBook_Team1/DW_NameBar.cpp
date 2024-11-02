#include "DW_NameBar.h"

/// ���̵�� ���� �޼���
void DrowWindow::createWindowSB(int left, int top, int right, int bottom, HWND parent)
{
    /// ������ ����� �ѹ��� �ؾ��ϱ� ������ ������ ��� �ڵ�� �ѹ��� ����

    static bool isClassRegistered = false;  /// Ŭ������ �̹� ��ϵǾ����� Ȯ��

    if (!isClassRegistered) {
        WNDCLASS wc5 = {};
        wc5.lpfnWndProc = WndProcSB;  /// ���ӹ� �޼��� ó���ϴ� ���� �޼���
        wc5.lpszClassName = L"CustomNameWindowClass2";
        wc5.hInstance = hInst;
        wc5.hbrBackground = CreateSolidBrush(RGB(230, 230, 230));

        if (!RegisterClass(&wc5)) {
            MessageBox(NULL, L"side �� ��� ����", L"Error", MB_OK);
            return;
        }

        isClassRegistered = true;  // Ŭ������ ��ϵ��� ǥ��
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
        reinterpret_cast<LPVOID>(this)  // this ������ ����
    );
    if (!WndFunc::sideWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"���̵� �� ���� ����: ���� �ڵ� %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::sideWnd, SW_SHOW);
}

/// ���� �� ���� �޼���
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
        return pThis->handleMessageNB(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
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

/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SETTEXT:
        /// WM_SETTEXT �޽��� ó��
        /// save�� �ε�� namebar �ؽ�Ʈ ����
        SetWindowText(fileNameW, reinterpret_cast<LPCWSTR>(lParam));
        break;
    case WM_CREATE:
    {
        /*
        backB = CreateWindow(L"BUTTON", L"<", WS_CHILD | WS_VISIBLE,
            10, 12, 30, 30, hWnd, (HMENU)NB_BACK_BT, nullptr, NULL);
*/
        fileNameW = CreateWindow(L"STATIC", L"�̸� ����", WS_CHILD | WS_VISIBLE,
            50, 12, 300, 30, hWnd, (HMENU)NB_FILE_NAME, nullptr, NULL);

        sideMenu.setCoordinate(WndFunc::wndSize.right - 40, 10, WndFunc::wndSize.right - 10, 40);

        HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, TEXT("�������"));

        // STATIC ��Ʈ�ѿ� ��Ʈ ����
        SendMessage(fileNameW, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;
    }
    case WM_SIZE:
    {
        /// â ũ�� ��ȭ�� ���̵� �޴� ��ư �̵�
        sideMenu.setCoordinate(WndFunc::wndSize.right - 40, 10, WndFunc::wndSize.right - 10, 40);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        /// ��ư Ŭ�� Ȯ�ο� ���콺 ��ǥ ���� RECT ����
        mousePoint.left = LOWORD(lParam);
        mousePoint.top = HIWORD(lParam);
        mousePoint.right = mousePoint.left + 1;
        mousePoint.bottom = mousePoint.top + 1;

        /// ���̵� ������ ���� �� â ����
        if (IntersectRect(&a1, &mousePoint, &sideMenu.rectButton)) {
            /// ��� ���� ����(��ư �̹��� ����)
            sideMenu.toggleState = !sideMenu.toggleState;

            /// ���� ���̵�ٰ� �������� ������ ����
            if (WndFunc::sideWnd == nullptr) {
                createWindowSB(WndFunc::wndSize.right - 60, 110, 60, 300, WndFunc::drowWnd);
            }
            else {
                /// ���ϸŴ����� ����Ʈ �ڽ� ����
                ShowWindow(DW_FileManager::hListBox, SW_HIDE); 
                ShowWindow(WndFunc::fileManager, SW_HIDE);
                /// ���̵� ������ DestroyWindow
                DestroyWindow(WndFunc::sideWnd);
                /// ���̵� ������ �ڵ鰪 �ʱ�ȭ
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

        /// ���̵� ��ư�� �̹��� ��ư �ΰ� 
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


