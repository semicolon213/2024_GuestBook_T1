#include "DW_ToolMenu.h"
#include "PenThickness.h"

DW_ToolMenu::DW_ToolMenu(HINSTANCE hInstance)
	:ChildWindow(cInst, RGB(249, 249, 249))
{
	tInst = hInstance;
    ToolRT = { 0 };
    tWnd = nullptr;
}

void DW_ToolMenu::Create(HWND hParentWnd, int x, int y, int width, int height)
{
	ChildWindow::Create(hParentWnd, L"DW_ToolMenuClass", L"Tool Child Window", x, y, width, height);
	tWnd = cWnd;
}

LRESULT DW_ToolMenu::HandleMessage(HWND tWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        ToolRT = ChildWindow::GetRT();
    case WM_CREATE:



        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            

<<<<<<< Updated upstream
        }
        break;

    default:
        return ChildWindow::HandleMessage(tWnd, message, wParam, lParam);
    }
    return 0;
=======
		case TL_PEN_WIDTH_BT:
		{
			PenThickness penThickness;
			penThickness.Show(tInst, Function::hWnd);
		}
		break;




			break;

		case TL_CLEAR_BT:
			if (function->getDrawLInfoEmpty())  break;
			if (!function->getIsReplay())
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

					SetWindowText(PlayBT, L"¡«");
					pCnt = FALSE;
				}
			}
			else
			{
				SendMessage(Function::hWnd, WM_COMMAND, TL_PLAY_BT, 1);

				SetWindowText(PlayBT, L"¢º");
				pCnt = true;
			}

			InvalidateRect(tWnd, nullptr, true);

			break;

		case TL_RESET_BT:
			if (function->getIsReplay())
				SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 0);
			else
				SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 1);

			SetWindowText(PlayBT, L"¢º");
			pCnt = true;

			break;


		default:
			break;
		}

		break;

	case WM_PAINT:


		toolRT = ChildWindow::GetRT();
		tHdc = GetDC(tWnd);
		tHdc = BeginPaint(tWnd, &t_ps);

		ToolBrush = (HBRUSH)SelectObject(tHdc, GetStockObject(NULL_BRUSH));
		ToolPen = (HPEN)SelectObject(tHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
		Rectangle(tHdc, toolRT.left - 1, toolRT.top, toolRT.right + 1, toolRT.bottom);

		if (*tCnt)
		{
			ShowWindow(Color1BT, SW_SHOW);
			ShowWindow(Color2BT, SW_SHOW);
			ShowWindow(Color3BT, SW_SHOW);
			ShowWindow(PenWidthBT, SW_SHOW);
			ShowWindow(ClearBT, SW_SHOW);
			ShowWindow(SaveBT, SW_SHOW);

			MoveWindow(Color1BT, (toolRT.right / 2) - 150, 10, 30, 30, true);
			MoveWindow(Color2BT, (toolRT.right / 2) - 100, 10, 30, 30, true);
			MoveWindow(Color3BT, (toolRT.right / 2) - 50, 10, 30, 30, true);
			MoveWindow(PenWidthBT, toolRT.right / 2, 10, 30, 30, true);
			MoveWindow(ClearBT, (toolRT.right / 2) + 50, 10, 30, 30, true);
			MoveWindow(SaveBT, toolRT.right - 50, 10, 30, 30, true);

			MoveToEx(tHdc, (toolRT.right / 2) + 98, 8, nullptr);
			LineTo(tHdc, (toolRT.right / 2) + 98, 42);

			MoveWindow(PlayBT, (toolRT.right / 2) + 115, 10, 30, 30, true);
			MoveWindow(ResetBT, (toolRT.right / 2) + 165, 10, 30, 30, true);
		}
		else
		{
			ShowWindow(Color1BT, SW_HIDE);
			ShowWindow(Color2BT, SW_HIDE);
			ShowWindow(Color3BT, SW_HIDE);
			ShowWindow(PenWidthBT, SW_HIDE);
			ShowWindow(ClearBT, SW_HIDE);
			ShowWindow(SaveBT, SW_HIDE);

			MoveWindow(PlayBT, (toolRT.right / 2) - 50, 10, 30, 30, true);
			MoveWindow(ResetBT, (toolRT.right / 2) + 20, 10, 30, 30, true);


		}


		SelectObject(tHdc, ToolPen);
		SelectObject(tHdc, ToolBrush);


		DeleteObject(ToolPen);
		DeleteObject(ToolBrush);
		EndPaint(tWnd, &t_ps);

	default:
		return ChildWindow::HandleMessage(tWnd, message, wParam, lParam);
	}
	return 0;
>>>>>>> Stashed changes
}