#include "DW_ToolMenu.h"
#include "PenThickness.h"





DW_ToolMenu::DW_ToolMenu(HINSTANCE hInstance)
	:ChildWindow(RGB(255, 255, 255))
{
	tInst = hInstance;
	toolRT = { 0 };
	tWnd = nullptr;
	tCnt = nullptr;
	pCnt = true;
	roundRgn = { 0 };
}

void DW_ToolMenu::Create(HWND hParentWnd, int x, int y, int width, int height)
{
	ChildWindow::Create(hParentWnd, L"DW_ToolMenuClass", L"Tool Child Window", x, y, width, height);
	tWnd = cWnd;

	toolRT = GetRT();

	/*
	pencilBT = CreateWindowW(L"BUTTON", L"BS", WS_CHILD | WS_VISIBLE,
		90, 10, 30, 30, tWnd, (HMENU)PENCIL, tInst, nullptr);
	sprayBT = CreateWindowW(L"BUTTON", L"SP", WS_CHILD | WS_VISIBLE,
		130, 10, 30, 30, tWnd, (HMENU)SPRAY, tInst, nullptrw);
	markerBT = CreateWindowW(L"BUTTON", L"MK", WS_CHILD | WS_VISIBLE,
		170, 10, 30, 30, tWnd, (HMENU)MARKER, tInst, nullptr);
			*/
	watercolorBT = CreateWindowW(L"BUTTON", L"BS", WS_CHILD | WS_VISIBLE,
		210, 10, 30, 30, tWnd, (HMENU)WATERCOLOR, tInst, nullptr);
	
	Color1BT = CreateWindowW(L"BUTTON", L"색1", WS_CHILD | WS_VISIBLE,
		(toolRT.right / 2) - 100, 10, 30, 30, tWnd, (HMENU)TL_COLOR1_BT, tInst, nullptr);
	Color2BT = CreateWindowW(L"BUTTON", L"색2", WS_CHILD | WS_VISIBLE,
		(toolRT.right / 2) - 50, 10, 30, 30, tWnd, (HMENU)TL_COLOR2_BT, tInst, nullptr);
	Color3BT = CreateWindowW(L"BUTTON", L"색3", WS_CHILD | WS_VISIBLE,
		toolRT.right / 2, 10, 30, 30, tWnd, (HMENU)TL_COLOR3_BT, tInst, nullptr);
	PenWidthBT = CreateWindowW(L"BUTTON", L"굵기", WS_CHILD | WS_VISIBLE,
		(toolRT.right / 2) + 50, 10, 30, 30, tWnd, (HMENU)TL_PEN_WIDTH_BT, tInst, nullptr);
	ClearBT = CreateWindowW(L"BUTTON", L"X", WS_CHILD | WS_VISIBLE,
		(toolRT.right / 2) + 100, 10, 30, 30, tWnd, (HMENU)TL_CLEAR_BT, tInst, nullptr);
	SaveBT = CreateWindowW(L"BUTTON", L"저장", WS_CHILD | WS_VISIBLE,
		toolRT.right - 50, 10, 30, 30, tWnd, (HMENU)TL_SAVE_BT, tInst, nullptr);

	PlayBT = CreateWindowW(L"BUTTON", L"▶", WS_CHILD | WS_VISIBLE,
		(toolRT.right / 2) - 50, 10, 30, 30, tWnd, (HMENU)TL_PLAY_BT, tInst, nullptr);
	ResetBT = CreateWindowW(L"BUTTON", L"■", WS_CHILD | WS_VISIBLE,
		(toolRT.right / 2) + 20, 10, 30, 30, tWnd, (HMENU)TL_RESET_BT, tInst, nullptr);

	roundRgn = CreateEllipticRgn(0, 0, 30, 30);

	
}

PAINTSTRUCT t_ps = { 0 };
HBRUSH ToolBrush = nullptr;
HPEN ToolPen = nullptr;
HDC tHdc = nullptr;

HPEN tt1, tt2;

MakeButton t1(10, 10, 40, 40);
MakeButton t2(50, 10, 80, 40);


RECT a;
RECT mouse;

LRESULT DW_ToolMenu::HandleMessage(HWND tWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
		toolRT = ChildWindow::GetRT();

	case WM_CREATE:
	{
		function = make_unique<Function>();
		colorPalette = make_unique<ColorPalette>();
		break;
	}
	case WM_LBUTTONDOWN:
	{
		/// 현재 마우스 좌표를 기준으로 사각형을 만들어 Rect 버튼과 겹침 확인
		mouse.left = LOWORD(lParam);
		mouse.top = HIWORD(lParam);
		mouse.right = mouse.left + 1;
		mouse.bottom = mouse.top + 1;

		/// 기본 펜
		if (IntersectRect(&a, &mouse, &t1.rectButton)) {
			function->setBShape(BASIC);
			break;
		}
		/// 펜슬
		if (IntersectRect(&a, &mouse, &t2.rectButton)) {
			function->setBShape(PENCIL);
			break;
		}

		break;
	}
	case WM_COMMAND:
		switch (wParam)
		{
		case TL_COLOR1_BT:
			if (Function::penNum == 0)
				colorPalette->colorSelect(tWnd, 0);
			else Function::penNum = 0;
			break;

		case TL_COLOR2_BT:
			if (Function::penNum == 1)
				colorPalette->colorSelect(tWnd, 1);
			else Function::penNum = 1;
			break;

		case TL_COLOR3_BT:

			if (Function::penNum == 2)
				colorPalette->colorSelect(tWnd, 2);
			else Function::penNum = 2;
			break;

		case TL_PEN_WIDTH_BT:
		{
			PenThickness penThickness;
			penThickness.show(tInst, Function::hWnd);
		}
		break;


		case BASIC:
			function->setBShape(BASIC);
			break;

		case BRUSH:
			function->setBShape(BRUSH);
			break;

		case PENCIL:
			function->setBShape(PENCIL);
			break;

		case SPRAY:
			function->setBShape(SPRAY);
			break;

		case MARKER:
			function->setBShape(MARKER);
			break;

		case WATERCOLOR:
			function->setBShape(WATERCOLOR);
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

	/// 툴 메뉴 바의 PAINT 영역
	case WM_PAINT:
		toolRT = ChildWindow::GetRT();
		//tHdc = GetDC(tWnd);
		tHdc = BeginPaint(tWnd, &t_ps);

		tt1 = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		tt2 = (HPEN)SelectObject(tHdc, tt1);


		t1.drawRectButton(tHdc, 551);
		t2.drawRectButton(tHdc, 552);

		SelectObject(tHdc, tt2);
		DeleteObject(tt1);

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
}