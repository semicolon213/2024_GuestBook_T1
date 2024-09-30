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
        (ToolRT.right / 2) - 100, 8, 30, 30, tWnd, (HMENU)TL_COLOR1_BT, tInst, nullptr);
    Color2BT = CreateWindowW(L"BUTTON", L"색2", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) - 50, 8, 30, 30, tWnd, (HMENU)TL_COLOR2_BT, tInst, nullptr);
    Color3BT = CreateWindowW(L"BUTTON", L"색3", WS_CHILD | WS_VISIBLE,
        ToolRT.right / 2, 8, 30, 30, tWnd, (HMENU)TL_COLOR3_BT, tInst, nullptr);
    PenWidthBT = CreateWindowW(L"BUTTON", L"굵기", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) + 50, 8, 30, 30, tWnd, (HMENU)TL_PEN_WIDTH_BT, tInst, nullptr);
    ClearBT = CreateWindowW(L"BUTTON", L"X", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) + 100, 8, 30, 30, tWnd, (HMENU)TL_CLEAR_BT, tInst, nullptr);
    SaveBT = CreateWindowW(L"BUTTON", L"저장", WS_CHILD | WS_VISIBLE,
        ToolRT.right - 50, 8, 30, 30, tWnd, (HMENU)TL_SAVE_BT, tInst, nullptr);

    PlayBT = CreateWindowW(L"BUTTON", L"▶", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) - 50, 8, 30, 30, tWnd, (HMENU)TL_PLAY_BT, tInst, nullptr);
    ResetBT = CreateWindowW(L"BUTTON", L"■", WS_CHILD | WS_VISIBLE,
        (ToolRT.right / 2) + 20, 8, 30, 30, tWnd, (HMENU)TL_RESET_BT, tInst, nullptr);


}

PAINTSTRUCT t_ps = { 0 };
HBRUSH ToolBrush = nullptr;
HPEN ToolPen = nullptr;
HDC tHdc = nullptr;

LRESULT DW_ToolMenu::HandleMessage(HWND tWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        ToolRT = ChildWindow::GetRT();

    case WM_CREATE:
        function = make_unique<Function>();
        break;

    case WM_COMMAND:

        switch (wParam)
        {
        case TL_COLOR1_BT:

            if (message == WM_LBUTTONDBLCLK)
            {
                /*
                버튼을 더블클릭했을때

                여기다가 컬러팔레트 기능 넣어주세용

                */
            }




            break;

        case TL_COLOR2_BT:

            if (message == WM_LBUTTONDBLCLK)
            {
                /*
                버튼을 더블클릭했을때

                여기다가 컬러팔레트 기능 넣어주세용

                */
            }




            break;

        case TL_COLOR3_BT:

            if (message == WM_LBUTTONDBLCLK)
            {
                /*
                버튼을 더블클릭했을때

                여기다가 컬러팔레트 기능 넣어주세용

                */
            }




            break;

        case TL_PEN_WIDTH_BT:

            if (message == WM_LBUTTONDBLCLK)
            {
                /*
                버튼을 더블클릭했을때

                여기다가 굵기 선택 기능 넣어주세용

                */
            }




            break;

        case TL_CLEAR_BT:
            if (function->getDrawLInfoEmpty())  break;
                SendMessage(Function::hWnd, WM_COMMAND, TL_CLEAR_BT, 0);
            break;


        case TL_SAVE_BT:

            break;

        case TL_PLAY_BT:
            if (function->getDrawLInfoEmpty())  break;
            if (pCnt)
            {
                if (!function->getIsReplay())
                {
                    SendMessage(Function::hWnd, WM_COMMAND, TL_PLAY_BT, 0);
                    
                    SetWindowText(PlayBT, L"∥");
                    pCnt = FALSE;
                }
            }
            else
            {
                SendMessage(Function::hWnd, WM_COMMAND, TL_PLAY_BT, 1);
                
                SetWindowText(PlayBT, L"▶");
                pCnt = true;
            }

            InvalidateRect(tWnd, nullptr, true);

            break;

        case TL_RESET_BT:

            if (function->getIsReplay())
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 0);
            else
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 1);

            SetWindowText(PlayBT, L"▶");
            pCnt = true;

            break;


        default:
            break;
        }

        break;

    case WM_PAINT:


        ToolRT = ChildWindow::GetRT();
        tHdc = GetDC(tWnd);
        tHdc = BeginPaint(tWnd, &t_ps);
        ToolBrush = (HBRUSH)SelectObject(tHdc, GetStockObject(NULL_BRUSH));
        ToolPen = (HPEN)SelectObject(tHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
        Rectangle(tHdc, ToolRT.left - 1, ToolRT.top, ToolRT.right + 1, ToolRT.bottom);

        if (*tCnt)
        {
            ShowWindow(Color1BT, SW_SHOW);
            ShowWindow(Color2BT, SW_SHOW);
            ShowWindow(Color3BT, SW_SHOW);
            ShowWindow(PenWidthBT, SW_SHOW);
            ShowWindow(ClearBT, SW_SHOW);
            ShowWindow(SaveBT, SW_SHOW);

            MoveWindow(Color1BT, (ToolRT.right / 2) - 150, 8, 30, 30, true);
            MoveWindow(Color2BT, (ToolRT.right / 2) - 100, 8, 30, 30, true);
            MoveWindow(Color3BT, (ToolRT.right / 2) - 50, 8, 30, 30, true);
            MoveWindow(PenWidthBT, ToolRT.right / 2, 8, 30, 30, true);
            MoveWindow(ClearBT, (ToolRT.right / 2) + 50, 8, 30, 30, true);
            MoveWindow(SaveBT, ToolRT.right - 50, 8, 30, 30, true);

            MoveToEx(tHdc, (ToolRT.right / 2) + 98, 5, nullptr);
            LineTo(tHdc, (ToolRT.right / 2) + 98, 40);

            MoveWindow(PlayBT, (ToolRT.right / 2) + 115, 8, 30, 30, true);
            MoveWindow(ResetBT, (ToolRT.right / 2) + 165, 8, 30, 30, true);
        }
        else
        {
            ShowWindow(Color1BT, SW_HIDE);
            ShowWindow(Color2BT, SW_HIDE);
            ShowWindow(Color3BT, SW_HIDE);
            ShowWindow(PenWidthBT, SW_HIDE);
            ShowWindow(ClearBT, SW_HIDE);
            ShowWindow(SaveBT, SW_HIDE);

            MoveWindow(PlayBT, (ToolRT.right / 2) - 50, 8, 30, 30, true);
            MoveWindow(ResetBT, (ToolRT.right / 2) + 20, 8, 30, 30, true);


        }


        SelectObject(tHdc, ToolPen);
        SelectObject(tHdc, ToolBrush);






        EndPaint(tWnd, &t_ps);

    default:
        return ChildWindow::HandleMessage(tWnd, message, wParam, lParam);
    }
    return 0;
}