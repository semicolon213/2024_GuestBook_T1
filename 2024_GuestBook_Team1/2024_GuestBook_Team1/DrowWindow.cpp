#include "DrowWindow.h"

DrowWindow::DrowWindow(HINSTANCE hInstance)
	:ChildWindow(cInst, RGB(243, 243, 243))
{
	dInst = hInstance;
    DrowRT = { 0 };
    dWnd = nullptr;
}

DW_ToolMenu* ToolMenu = nullptr;
HWND t_hWnd = nullptr;

void DrowWindow::Create(HWND hParentWnd, int x, int y, int width, int height) 
{
	ChildWindow::Create(hParentWnd, L"BlueWindowClass", L"Blue Child Window", x, y, width, height);
	dWnd = cWnd;

    DrowRT = ChildWindow::GetRT();

    ToolMenu = new DW_ToolMenu(dInst);
    ToolMenu->Create(dWnd, -1, 52, DrowRT.right, 46);
    /*ToolMenu->Show(true);*/
    t_hWnd = ToolMenu->GetHWND();
}


HDC pHdc = nullptr;
PAINTSTRUCT ps = { 0 };
HPEN partition = nullptr;
HPEN oldpen = nullptr;

LRESULT DrowWindow::HandleMessage(HWND dWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        DrowRT = ChildWindow::GetRT();


    case WM_CREATE:
        DrowRT = ChildWindow::GetRT();
        // 뒤로가기 버튼 생성
        CreateWindow(L"BUTTON", L"<", WS_CHILD | WS_VISIBLE,
            10, 10, 30, 30, dWnd, (HMENU)1, dInst, NULL);



        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) { // Back 버튼 클릭 시
            Show(false); // 자식 윈도우 숨기기
            EnableWindow(GetDlgItem(GetParent(dWnd), BUTTON_ID), TRUE);
        }
        break;

    case WM_PAINT:
        DrowRT = ChildWindow::GetRT();
        pHdc = BeginPaint(dWnd, &ps);
        RECT ToolRT = this->GetChildPos(dWnd, t_hWnd);
        partition = CreatePen(PS_SOLID, 1, RGB(234, 234, 234));
        oldpen = (HPEN)SelectObject(pHdc, partition);
        Rectangle(pHdc, ToolRT.left, ToolRT.top, ToolRT.right, ToolRT.bottom);
        SelectObject(pHdc, oldpen);
        DeleteObject(partition);
        EndPaint(dWnd, &ps);
        break;

    default:
        return ChildWindow::HandleMessage(dWnd, message, wParam, lParam);
    }
    return 0;
}