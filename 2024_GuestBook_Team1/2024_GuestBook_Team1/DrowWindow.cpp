#include "DrowWindow.h"

DrowWindow::DrowWindow(HINSTANCE hInstance)
	:ChildWindow(RGB(249, 243, 240))
{
	dInst = hInstance;
    DrowRT = { 0 };
    dWnd = nullptr;
    ToolCnt = TRUE;

    SideMenu = nullptr;
    s_hWnd = nullptr;
}

DW_NameBar* NameBar = nullptr;
HWND n_hWnd = nullptr;


DW_ToolMenu* ToolMenu = nullptr;
HWND t_hWnd = nullptr;

DW_Canvas* Canvas = nullptr;
HWND C_hWnd = nullptr;

void DrowWindow::Create(HWND hParentWnd, int x, int y, int width, int height) 
{
	ChildWindow::Create(hParentWnd, L"BlueWindowClass", L"Blue Child Window", x, y, width, height);
	dWnd = cWnd;


    DrowRT = ChildWindow::GetRT();

    NameBar = new DW_NameBar(dInst);
    NameBar->Create(dWnd, 0, 0, DrowRT.right, 52);
    n_hWnd = NameBar->GetHWND();

    ToolMenu = new DW_ToolMenu(dInst);
    ToolMenu->Create(dWnd, -1, 52, DrowRT.right, 46);
    //ToolMenu->Show(true);
    t_hWnd = ToolMenu->GetHWND();
    ToolCnt = TRUE;
    ToolMenu->tCnt = &ToolCnt;
    NameBar->tCnt = &ToolCnt;

    Canvas = new DW_Canvas(dInst);
    Canvas->Create(dWnd, (DrowRT.right - 1300) / 2, (DrowRT.bottom - 750) / 2 + 75, 1300, 700);
    C_hWnd = Canvas->GetHWND();


    SideMenu = new DW_SideMenu(dInst);
    SideMenu->Create(dWnd, DrowRT.right - 300, 0, 300, 600);
    s_hWnd = SideMenu->GetHWND();

    SideMenu->Show(FALSE);

}


HDC pHdc = nullptr;
PAINTSTRUCT d_ps = { 0 };
HPEN partition = nullptr;
HPEN DrowPen = nullptr;
HBRUSH DrowBrush = nullptr;

LRESULT DrowWindow::HandleMessage(HWND dWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        DrowRT = ChildWindow::GetRT();


    case WM_CREATE:
        DrowRT = ChildWindow::GetRT();
        // 뒤로가기 버튼 생성
        /*CreateWindow(L"BUTTON", L"<", WS_CHILD | WS_VISIBLE,
            10, 10, 30, 30, dWnd, (HMENU)1, dInst, NULL)*/;



        break;
    case WM_SIZE:
        DrowRT = ChildWindow::GetRT();

        MoveWindow(n_hWnd, 0, 0, DrowRT.right, 52,TRUE);

        MoveWindow(t_hWnd, -1, 52, DrowRT.right, 46, TRUE);

        MoveWindow(C_hWnd, (DrowRT.right - 1300) / 2, (DrowRT.bottom - 750) / 2 + 75, 1300, 700, TRUE);

        MoveWindow(s_hWnd, DrowRT.right - 350, 0, 350, 600, true);

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case NB_BACK_BT:
            Show(false); // 자식 윈도우 숨기기
            EnableWindow(GetDlgItem(GetParent(dWnd), DEF_DROW_BT), TRUE);
            EnableWindow(GetDlgItem(GetParent(dWnd), DEF_LOAD_BT), TRUE);
            EnableWindow(GetDlgItem(GetParent(dWnd), DEF_CREDIT_BT), TRUE);
            EnableWindow(dWnd, FALSE);
            break;

        case NB_SIDE_BT:
            SideMenu->Show(true);
            SetFocus(s_hWnd);
            InvalidateRect(s_hWnd, NULL, true);
            break;

        default:
            break;
        }
        
        break;

    case WM_PAINT:
        DrowRT = ChildWindow::GetRT();
        pHdc = BeginPaint(dWnd, &d_ps);
        RECT ToolRT = this->GetChildPos(dWnd, t_hWnd);
        EndPaint(dWnd, &d_ps);
        break;

    default:
        return ChildWindow::HandleMessage(dWnd, message, wParam, lParam);
    }
    return 0;
}