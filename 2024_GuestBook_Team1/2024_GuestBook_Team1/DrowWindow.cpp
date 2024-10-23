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

    desktopRT = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    GetWindowRect(GetParent(dWnd), &MainRT);

    drowRT = ChildWindow::GetRT();

    nameBar = make_unique<DW_NameBar>(dInst);
    nameBar->Create(dWnd, 0, 0, drowRT.right, 57);
    nHWnd = nameBar->GetHWND();

    toolMenu = make_unique<DW_ToolMenu>(dInst);
    toolMenu->Create(dWnd, -1, 57, drowRT.right, 51);
    //toolMenu->show(true);
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

    ConnExcel::list = connExcel->getVisitList().c_str();

    connExcel->setTextPosX(drowRT.right);

}



LRESULT DrowWindow::HandleMessage(HWND dWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {

    case WM_SIZE:
        drowRT = GetRT();


        MoveWindow(nHWnd, 0, 0, drowRT.right, 57, TRUE);

        MoveWindow(tHWnd, -1, 57, drowRT.right, 51, TRUE);

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
            GetClientRect(dWnd, &drowRT);
            pt = { 0 };
            ClientToScreen(dWnd, &pt);
            IntersectRect(&drowRT, &desktopRT, &drowRT);

            DSideRT = GetChildPos(dWnd, sHWnd);

            MoveWindow(sHWnd, pt.x + drowRT.right - 351, pt.y + drowRT.top, 350, 600, true);
            sideMenu->Show(true);
            break;

        case TL_PLAY_BT:
            SendMessage(tHWnd, WM_COMMAND, TL_PLAY_BT, 0); /// 이거 바뀜
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
        SIZE textSize;
        wsprintf(text, ConnExcel::list.c_str());
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
