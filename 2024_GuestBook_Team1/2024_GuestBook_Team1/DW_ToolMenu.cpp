#include "DW_ToolMenu.h"
DW_ToolMenu::DW_ToolMenu(HINSTANCE hInstance)
	:ChildWindow(RGB(249, 249, 249))
{
	tInst = hInstance;
    ToolRT = { 0 };
    tWnd = nullptr;
    tCnt = nullptr;
    pCnt = true;
}

void DW_ToolMenu::Create(HWND hParentWnd, int x, int y, int width, int height)
{
	ChildWindow::Create(hParentWnd, L"DW_ToolMenuClass", L"Tool Child Window", x, y, width, height);
	tWnd = cWnd;

    ToolRT = GetRT();

    Color1BT = CreateWindowW(L"BUTTON", L"색1", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) - 80, 10, 30, 30, tWnd, (HMENU)TL_COLOR1_BT, tInst, nullptr);
    Color2BT = CreateWindowW(L"BUTTON", L"색2", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) - 40, 10, 30, 30, tWnd, (HMENU)TL_COLOR2_BT, tInst, nullptr);
    Color3BT = CreateWindowW(L"BUTTON", L"색3", WS_CHILD | WS_VISIBLE,
        ToolRT.right / 2, 10, 30, 30, tWnd, (HMENU)TL_COLOR3_BT, tInst, nullptr);
    PenWidthBT = CreateWindowW(L"BUTTON", L"굵기", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) + 40, 10, 30, 30, tWnd, (HMENU)TL_PEN_WIDTH_BT, tInst, nullptr);
    ClearBT = CreateWindowW(L"BUTTON", L"X", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) + 80, 10, 30, 30, tWnd, (HMENU)TL_CLEAR_BT, tInst, nullptr);
    SaveBT = CreateWindowW(L"BUTTON", L"저장", WS_CHILD | WS_VISIBLE,
        ToolRT.right - 50, 10, 30, 30, tWnd, (HMENU)TL_SAVE_BT, tInst, nullptr);

    PlayBT = CreateWindowW(L"BUTTON", L"▶", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) - 50, 10, 30, 30, tWnd, (HMENU)TL_PLAY_BT, tInst, nullptr);
    ResetBT = CreateWindowW(L"BUTTON", L"■", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) + 20, 10, 30, 30, tWnd, (HMENU)TL_RESET_BT, tInst, nullptr);


}

PAINTSTRUCT t_ps = { 0 };
HBRUSH ToolBrush = nullptr;
HPEN ToolPen = nullptr;
HDC tHdc = nullptr;

LRESULT DW_ToolMenu::HandleMessage(HWND tWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        ToolRT = ChildWindow::GetRT();
  
    case WM_COMMAND:
        
        switch (wParam)
        {
        case TL_COLOR1_BT:

            break;

        case TL_COLOR2_BT:

            break;

        case TL_COLOR3_BT:

            break;

        case TL_PEN_WIDTH_BT:

            break;

        case TL_CLEAR_BT:

            break;

        case TL_SAVE_BT:

            break;

        case TL_PLAY_BT:
            
            if (pCnt)
            {
                SetWindowText(PlayBT, L"∥");
                pCnt = FALSE;
            }
            else
            {
                SetWindowText(PlayBT, L"▶");
                pCnt = true;
            }

            InvalidateRect(tWnd, nullptr, true);

            break;

        case TL_RESET_BT:
            SetWindowText(PlayBT, L"▶");
            pCnt = true;

            break;


        default:
            break;
        }

        break;
    case WM_PAINT:

        if (*tCnt)
        {
            ShowWindow(Color1BT, SW_SHOW);
            ShowWindow(Color2BT, SW_SHOW);
            ShowWindow(Color3BT, SW_SHOW);
            ShowWindow(PenWidthBT, SW_SHOW);
            ShowWindow(ClearBT, SW_SHOW);
            ShowWindow(SaveBT, SW_SHOW);

            ShowWindow(PlayBT, SW_HIDE);
            ShowWindow(ResetBT, SW_HIDE);
        }
        else
        {
            ShowWindow(Color1BT, SW_HIDE);
            ShowWindow(Color2BT, SW_HIDE);
            ShowWindow(Color3BT, SW_HIDE);
            ShowWindow(PenWidthBT, SW_HIDE);
            ShowWindow(ClearBT, SW_HIDE);
            ShowWindow(SaveBT, SW_HIDE);

            ShowWindow(PlayBT, SW_SHOW);
            ShowWindow(ResetBT, SW_SHOW);


        }

        ToolRT = ChildWindow::GetRT();
        tHdc = GetDC(tWnd);
        tHdc = BeginPaint(tWnd, &t_ps);
        ToolBrush = (HBRUSH)SelectObject(tHdc, GetStockObject(NULL_BRUSH));
        ToolPen = (HPEN)SelectObject(tHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
        Rectangle(tHdc, ToolRT.left - 1, ToolRT.top, ToolRT.right + 1, ToolRT.bottom);
        SelectObject(tHdc, ToolPen);
        SelectObject(tHdc, ToolBrush);






        EndPaint(tWnd, &t_ps);

    default:
        return ChildWindow::HandleMessage(tWnd, message, wParam, lParam);
    }
    return 0;
}