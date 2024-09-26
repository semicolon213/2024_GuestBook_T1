#include "DrowWindow.h"

DrowWindow::DrowWindow(HINSTANCE hInstance)
    :ChildWindow(RGB(249, 243, 240))
{
    dInst = hInstance;
    drowRT = { 0 };
    dWnd = nullptr;
    toolCnt = TRUE;

    sideMenu = nullptr;
    sHWnd = nullptr;
    pt = { 0 };
    desktopRT = { 0 };
}


void DrowWindow::Create(HWND hParentWnd, int x, int y, int width, int height) 
{
	ChildWindow::Create(hParentWnd, L"DrowWindow", L"DrowWindow", x, y, width, height);
	dWnd = cWnd;

    GetWindowRect(GetDesktopWindow(), &desktopRT);
    GetWindowRect(GetParent(dWnd), &MainRT);

    drowRT = ChildWindow::GetRT();

    nameBar = make_unique<DW_NameBar>(dInst);
    nameBar->Create(dWnd, 0, 0, drowRT.right, 52);
    nHWnd = nameBar->GetHWND();

    toolMenu = make_unique<DW_ToolMenu>(dInst);
    toolMenu->Create(dWnd, -1, 52, drowRT.right, 46);
    //toolMenu->Show(true);
    tHWnd = toolMenu->GetHWND();
    toolCnt = TRUE;
    toolMenu->tCnt = &toolCnt;
    nameBar->tCnt = &toolCnt;

    canvas = make_unique<DW_Canvas>(dInst);
    canvas->Create(dWnd, (drowRT.right - 1300) / 2, (drowRT.bottom - 750) / 2 + 75, 1300, 700);
    cHWnd = canvas->GetHWND();


    sideMenu = make_unique<DW_SideMenu>(dInst);
    sideMenu->CreatePop(dWnd, MainRT.right - 350, MainRT.top, 350, 600);
    sHWnd = sideMenu->GetHWND();

    sideMenu->Show(FALSE);

    connExcel = make_unique<ConnExcel>();
    
    connExcel->listScrollThread(dWnd, getDWWidth(), drowRT);

    list = connExcel->getVisitList().c_str();

    connExcel->setTextPosX(drowRT.right);

}



LRESULT DrowWindow::HandleMessage(HWND dWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {

    case WM_SIZE:
        drowRT = GetRT();
        
        /*SetWindowPos(nHWnd, HWND_BOTTOM, 0, 0, drowRT.right, 52, NULL);

        SetWindowPos(tHWnd, HWND_BOTTOM, -1, 52, drowRT.right, 98, NULL);

        SetWindowPos(cHWnd, HWND_BOTTOM, (drowRT.right - 1300) / 2, (drowRT.bottom - 750) / 2 + 75,
            (drowRT.right + 1300) / 2, (drowRT.bottom + 750) / 2 + 75, NULL);

        SetWindowPos(sHWnd, HWND_TOPMOST, drowRT.right - 350, 0, drowRT.right, 600, NULL);*/

        //InvalidateRect(dWnd, nullptr, true);

        MoveWindow(nHWnd, 0, 0, drowRT.right, 52,TRUE);

        MoveWindow(tHWnd, -1, 52, drowRT.right, 46, TRUE);

        MoveWindow(cHWnd, (drowRT.right - 1300) / 2, (drowRT.bottom - 750) / 2 + 75, 1300, 700, TRUE);


        InvalidateRect(dWnd, nullptr, true);

        break;

    case WM_MOVE:


        break;

    case WM_COMMAND:
        drowRT = GetRT();
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
            MoveWindow(sHWnd,drowRT.left-300,drowRT.top)*/
            sideMenu->Show(true);
            SetFocus(sHWnd);
            InvalidateRect(dWnd, NULL, true);
            InvalidateRect(sHWnd, NULL, true);
            break;

        default:
            break;
        }
        
        break;

    case WM_LBUTTONDOWN:
        drowRT = GetRT();
        

        break;

    case WM_PAINT:
        drowRT = ChildWindow::GetRT();
        pHdc = BeginPaint(dWnd, &d_ps);
        RECT toolRT = this->GetChildPos(dWnd, tHWnd);
        SIZE textSize;
        list = connExcel->getVisitList().c_str();
        wsprintf(text,list.c_str());
        SetBkColor(pHdc, RGB(249, 243, 240));
        TextOut(pHdc, connExcel->getTextPosX(), drowRT.bottom - 15, text, lstrlen(text));

        EndPaint(dWnd, &d_ps);
        break;

    default:
        return ChildWindow::HandleMessage(dWnd, message, wParam, lParam);
    }
    return 0;
}

int DrowWindow::getDWWidth()
{
    return drowRT.right - drowRT.left;
}
