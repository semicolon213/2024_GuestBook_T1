#include "DrowWindow.h"

DrowWindow::DrowWindow(HINSTANCE hInstance)
	:ChildWindow(cInst, RGB(243, 243, 243))
{
	dInst = hInstance;
}

void DrowWindow::Create(HWND hParentWnd, int x, int y, int width, int height) 
{
	ChildWindow::Create(hParentWnd, L"BlueWindowClass", L"Blue Child Window", x, y, width, height);
	dWnd = cWnd;
}

DW_ToolMenu* ToolMenu = nullptr;
HWND t_hWnd = nullptr;


LRESULT DrowWindow::HandleMessage(HWND dWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        DrowRT = ChildWindow::GetRT();
    case WM_CREATE:

        // �ڷΰ��� ��ư ����
        CreateWindow(L"BUTTON", L"<", WS_CHILD | WS_VISIBLE,
            10, 10, 30, 30, dWnd, (HMENU)1, dInst, NULL);

        ToolMenu = new DW_ToolMenu(dInst);



        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) { // Back ��ư Ŭ�� ��
            Show(false); // �ڽ� ������ �����
            EnableWindow(GetDlgItem(GetParent(dWnd), BUTTON_ID), TRUE);
        }
        break;

    default:
        return ChildWindow::HandleMessage(dWnd, message, wParam, lParam);
    }
    return 0;
}