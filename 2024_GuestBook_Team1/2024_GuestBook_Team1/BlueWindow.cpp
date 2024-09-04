#include "BlueWindow.h"

BlueWindow::BlueWindow(HINSTANCE hInstance)
    : ChildWindow(cInst, RGB(000, 000, 255)) 
{
    bInst = hInstance;
}

void BlueWindow::Create(HWND hParentWnd, int x, int y, int width, int height) {
    ChildWindow::Create(hParentWnd, L"BlueWindowClass", L"Blue Child Window", x, y, width, height);
    bWnd = cWnd;
}

LRESULT BlueWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        // �ڷΰ��� ��ư ����
        CreateWindow(L"BUTTON", L"Back", WS_CHILD | WS_VISIBLE,
                     10, 10, 100, 30, hWnd, (HMENU)1, bInst, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) { // Back ��ư Ŭ�� ��
            Show(false); // �ڽ� ������ �����
            EnableWindow(GetDlgItem(GetParent(hWnd), BUTTON_ID), TRUE);
        }
        break;

    default:
        return ChildWindow::HandleMessage(hWnd, message, wParam, lParam);
    }
    return 0;
}
