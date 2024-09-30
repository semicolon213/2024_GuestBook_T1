#include "DW_NameBar.h"
DW_NameBar::DW_NameBar(HINSTANCE hInstance)
    :ChildWindow(RGB(243, 243, 243))
{
    nInst = hInstance;
    NameRT = { 0 };
    nWnd = nullptr;
    BackB = nullptr;
    SideB = nullptr;
    FileNameW = nullptr;
    tCnt = nullptr;
}

void DW_NameBar::Create(HWND hParentWnd, int x, int y, int width, int height)
{
    ChildWindow::Create(hParentWnd, L"DW_NameBarClass", L"Name Child Window", x, y, width, height);
    nWnd = cWnd;

    NameRT = GetRT();


    BackB = CreateWindow(L"BUTTON", L"<", WS_CHILD | WS_VISIBLE,
        10, 12, 30, 30, nWnd, (HMENU)NB_BACK_BT, nullptr, NULL);

    FileNameW = CreateWindow(L"STATIC", L"이름 없음", WS_CHILD | WS_VISIBLE,
        50, 12, 300, 30, nWnd, (HMENU)NB_FILE_NAME, nullptr, NULL);

    SideB = CreateWindow(L"BUTTON", L":", WS_CHILD | WS_VISIBLE,
        NameRT.right - 40, 12, 30, 30, nWnd, (HMENU)NB_SIDE_BT, nullptr, NULL);

    HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, TEXT("나눔고딕"));

    // STATIC 컨트롤에 폰트 설정
    SendMessage(FileNameW, WM_SETFONT, (WPARAM)hFont, TRUE);
}

PAINTSTRUCT n_ps = { 0 };
HBRUSH NameBrush = nullptr;
HPEN NamePen = nullptr;
HDC nHdc = nullptr;

LRESULT DW_NameBar::HandleMessage(HWND tWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        NameRT = ChildWindow::GetRT();
    case WM_CREATE:

        

        break;

    case WM_SIZE:
        NameRT = GetRT();
        MoveWindow(SideB, NameRT.right - 40, 12, 30, 30, true);

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case NB_BACK_BT:
            SendMessage(GetParent(nWnd), message, wParam, lParam);


            /*ShowWindow(GetParent(nWnd), SW_HIDE);
            EnableWindow(GetDlgItem(GetParent(GetParent(nWnd)), BUTTON_ID), TRUE);*/
            break;

        case NB_SIDE_BT:
            SendMessage(GetParent(nWnd), message, wParam, lParam);
            break;

        default:
            break;
        }
        
        break;
    case WM_PAINT:
        NameRT = ChildWindow::GetRT();
        nHdc = GetDC(tWnd);
        nHdc = BeginPaint(tWnd, &n_ps);





        EndPaint(tWnd, &n_ps);

    default:
        return ChildWindow::HandleMessage(tWnd, message, wParam, lParam);
    }
    return 0;
}