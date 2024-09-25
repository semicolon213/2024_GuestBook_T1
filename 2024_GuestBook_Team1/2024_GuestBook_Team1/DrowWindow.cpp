


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
    pt = { 0 };
    DesktopRT = { 0 };
}


void DrowWindow::Create(HWND hParentWnd, int x, int y, int width, int height) 
{
	ChildWindow::Create(hParentWnd, L"BlueWindowClass", L"Blue Child Window", x, y, width, height);
	dWnd = cWnd;

    GetWindowRect(GetDesktopWindow(), &DesktopRT);
    GetWindowRect(GetParent(dWnd), &MainRT);

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
    SideMenu->CreatePop(dWnd, MainRT.right - 350, MainRT.top, 350, 600);
    s_hWnd = SideMenu->GetHWND();

    SideMenu->Show(FALSE);

}



LRESULT DrowWindow::HandleMessage(HWND dWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {

    case WM_SIZE:
        DrowRT = GetRT();
        
        /*SetWindowPos(n_hWnd, HWND_BOTTOM, 0, 0, DrowRT.right, 52, NULL);

        SetWindowPos(t_hWnd, HWND_BOTTOM, -1, 52, DrowRT.right, 98, NULL);

        SetWindowPos(C_hWnd, HWND_BOTTOM, (DrowRT.right - 1300) / 2, (DrowRT.bottom - 750) / 2 + 75,
            (DrowRT.right + 1300) / 2, (DrowRT.bottom + 750) / 2 + 75, NULL);

        SetWindowPos(s_hWnd, HWND_TOPMOST, DrowRT.right - 350, 0, DrowRT.right, 600, NULL);*/

        //InvalidateRect(dWnd, nullptr, true);

        MoveWindow(n_hWnd, 0, 0, DrowRT.right, 52,TRUE);

        MoveWindow(t_hWnd, -1, 52, DrowRT.right, 46, TRUE);

        MoveWindow(C_hWnd, (DrowRT.right - 1300) / 2, (DrowRT.bottom - 750) / 2 + 75, 1300, 700, TRUE);


        InvalidateRect(dWnd, nullptr, true);

        break;

    case WM_MOVE:


        break;

    case WM_COMMAND:
        DrowRT = GetRT();
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
            /*POINT pt;
            ClientToScreen(dWnd, &pt);
            MoveWindow(s_hWnd,DrowRT.left-300,DrowRT.top)*/
            SideMenu->Show(true);
            SetFocus(s_hWnd);
            InvalidateRect(dWnd, NULL, true);
            InvalidateRect(s_hWnd, NULL, true);
            break;

        default:
            break;
        }
        
        break;

    case WM_LBUTTONDOWN:
        DrowRT = GetRT();
        

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