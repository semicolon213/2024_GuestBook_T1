#include "DW_ColorBox.h"

DW_ColorBox::DW_ColorBox(HINSTANCE hInstance)
	: ChildWindow(RGB(243, 243, 243)), penMemory(new std::vector<PINFO>)
{
	bInst = hInstance;
	BoxRT = { 0 };
	bWnd = nullptr;

	colorPicker = new ColorPicker(bWnd);
}

void DW_ColorBox::CreatePop(HWND hParentWnd, int x, int y, int width, int height)
{
	ChildWindow::CreatePop(hParentWnd, L"DW_SideMenuClass", L"Side Child Window", x, y, width, height);
	bWnd = cWnd;
}

PAINTSTRUCT b_ps = { 0 };
HDC bHdc = nullptr;
HBRUSH BoxBrush = nullptr;
HPEN BoxPen = nullptr;

LRESULT DW_ColorBox::HandleMessage(HWND pWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_KILLFOCUS:

		ShowWindow(pWnd, false);

		break;
		/*
	case WM_PAINT:

		
		BoxRT = ChildWindow::GetRT();
		bHdc = GetDC(pWnd);
		bHdc = BeginPaint(pWnd, &b_ps);

		colorPicker->drawColors(bHdc);
		colorPicker->drawPreview(bHdc);
		colorPicker->showPicker(pWnd);
		colorPicker->Draw(bHdc);
		TextOut (bHdc, 10, 200, L"Thickness:", 9);

		BoxBrush = (HBRUSH)SelectObject(bHdc, GetStockObject(NULL_BRUSH));
		BoxPen = (HPEN)SelectObject(bHdc, CreatePen(PS_SOLID, 1, RGB(0, 234, 234)));
		Rectangle(bHdc, BoxRT.left, BoxRT.top, BoxRT.right, BoxRT.bottom);
		EndPaint(pWnd, &b_ps);
		
		*/
	case WM_LBUTTONDOWN: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		colorPicker->handleColorSelection(pWnd, x, y);
	}

	default:
		return ChildWindow::HandleMessage(pWnd, message, wParam, lParam);
}
return 0;
}