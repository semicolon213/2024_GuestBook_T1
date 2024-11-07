#include "DW_NameBar.h"

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
HWND sideB = nullptr;

MakeButton DW_NameBar::sideMenu;
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

        if (!WndFunc::creditOn) {
            SetWindowText(WndFunc::fileNameW, reinterpret_cast<LPCWSTR>(lParam));
        }
        break;
    case WM_CREATE:
    {
        WndFunc::fileNameW = CreateWindow(L"STATIC", L"�̸� ����", WS_CHILD | WS_VISIBLE,
            50, 12, 300, 30, hWnd, (HMENU)NB_FILE_NAME, nullptr, NULL);


        DW_NameBar::sideMenu.setCoordinate(WndFunc::wndSize.right - 40, 10, WndFunc::wndSize.right - 10, 40);

        HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, TEXT("�������"));

        // STATIC ��Ʈ�ѿ� ��Ʈ ����
        SendMessage(WndFunc::fileNameW, WM_SETFONT, (WPARAM)hFont, TRUE);

        break;
    }
    case WM_SIZE:
    {
        /// â ũ�� ��ȭ�� ���̵� �޴� ��ư �̵�
        DW_NameBar::sideMenu.setCoordinate(WndFunc::wndSize.right - 40, 10, WndFunc::wndSize.right - 10, 40);

        break;
    }
    case WM_LBUTTONDOWN:
    {
        if (function->getIsReplay()) {
            break;
        }

        /// ��ư Ŭ�� Ȯ�ο� ���콺 ��ǥ ���� RECT ����
        mousePoint.left = LOWORD(lParam);
        mousePoint.top = HIWORD(lParam);
        mousePoint.right = mousePoint.left + 1;
        mousePoint.bottom = mousePoint.top + 1;

        /// ���̵� ������ ���� �� â ����
         /// ���̵� �޴� ��ư�� Ŭ���ߴ��� Ȯ��
        if (IntersectRect(&a1, &mousePoint, &DW_NameBar::sideMenu.rectButton))
        {

            if (!IsWindowVisible(WndFunc::sideWnd))
            {
                DW_NameBar::sideMenu.toggleState = true;
                ShowWindow(WndFunc::sideWnd, SW_SHOW);
            }
            else
            {
                DW_NameBar::sideMenu.toggleState = false;
                ShowWindow(WndFunc::sideWnd, SW_HIDE);
            }
        }

        if (IntersectRect(&a1, &mousePoint, &exitButton.rectButton)) {
            WndFunc::buttonOn = true;
            WndFunc::creditOn = false;

            DW_NameBar::sideMenu.toggleState = false;
            SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)L"�̸� ����");

            ShowWindow(WndFunc::fileNameW, SW_SHOW);

            ShowWindow(WndFunc::drowWnd, SW_HIDE);
            ShowWindow(WndFunc::nameWnd, SW_HIDE);
            ShowWindow(WndFunc::toolWnd, SW_HIDE);
            ShowWindow(WndFunc::canvasWnd, SW_HIDE);
            ShowWindow(WndFunc::sideWnd, SW_HIDE);
            ShowWindow(WndFunc::visitListWnd, SW_HIDE);

            ShowWindow(WndFunc::DrowBT, SW_SHOW);
            ShowWindow(WndFunc::LoadBT, SW_SHOW);
            ShowWindow(WndFunc::CreditBT, SW_SHOW);

            function->clearDrawing(WndFunc::canvasWnd);
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
        if (!WndFunc::creditOn) {
            DW_NameBar::sideMenu.doubleImgButton(hdc, IDI_CLOSE_MENU_ICON, IDI_MENU_ICON);
        }
        exitButton.drawRectButton(hdc, IDI_EXIT_ICON);
        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


