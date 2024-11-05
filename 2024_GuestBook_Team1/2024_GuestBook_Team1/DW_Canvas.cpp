#include "DW_Canvas.h"

// ��� ���� �߰�
bool isDrawing = false;    // ����� ���� �÷���
POINT lastPoint;           // ���� ���� ��ǥ

/// ���� �� ���� �޼���
LRESULT CALLBACK DrowWindow::WndProcCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	DrowWindow* pThis = nullptr;

	if (message == WM_NCCREATE) {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		pThis = reinterpret_cast<DrowWindow*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else {
		pThis = reinterpret_cast<DrowWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis) {
		return pThis->handleMessageCV(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

wchar_t buffer[256];

/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
		static HDC memDC;
		static HBITMAP hBitmap;

	case WM_CREATE:
	{
		HDC hdc = GetDC(WndFunc::canvasWnd);
		memDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, WndFunc::wndSize.right, WndFunc::wndSize.bottom);
		SelectObject(memDC, hBitmap);
		ReleaseDC(WndFunc::canvasWnd, hdc);
		function = std::make_unique<Function>();
		function->GDIPlusStart(); // �� gdi ���̺귯�� Ȱ��ȭ

	}
	break;
	case WM_COMMAND:
	{
		if (wParam == TL_CLEAR_BT)
		{
			if (function->getIsReset())
			{
				function->clearDrawing(WndFunc::canvasWnd);
			}

		}

		if (wParam == TL_PLAY_BT && lParam == 0)
		{
			if (!function->getIsReplay())
			{
				function->replayThread(WndFunc::canvasWnd);
			}
			else
			{
				function->resumeReplay();
			}
		}


		if (wParam == TL_PLAY_BT && lParam == 1)
		{
			function->suspendReplay();
		}

		if (wParam == TL_RESET_BT)
		{
			function->reDrawing(WndFunc::canvasWnd);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		GetWindowText(WndFunc::fileNameW, buffer, sizeof(buffer) / sizeof(wchar_t));
		if (wcscmp(buffer, L"�̸� ����") != 0)
		{
			function->setisLeftClick(false);
			function->mouseUD(drawPInfo, FALSE);
			function->draw(WndFunc::canvasWnd, drawPInfo, FALSE);
			break;
		}

		if (!function->getIsReset()) break;
		//hdc = GetDC(canWnd);



		////////////////////////////////////////////////

		drawPInfo.lParam = lParam;
		drawPInfo.pColor = DW_ColorBox::getColorNum(DW_ColorBox::colorSelect);//ColorPalette::colorArr[Function::penNum];
		drawPInfo.pTime = (DWORD)GetTickCount64();
		drawPInfo.pWidth = penThickness->getPenWidth(); /// �� ���� ����
		drawPInfo.state = message;
		function->draw(WndFunc::canvasWnd, drawPInfo, TRUE); // �귯�� ��� �߰��Ϸ��� �ش� RECTANGLE �� �˸´� ������ ������ ��.
	}
	break;

	case WM_LBUTTONDOWN:

		GetWindowText(WndFunc::fileNameW, buffer, sizeof(buffer) / sizeof(wchar_t));
		if (wcscmp(buffer, L"�̸� ����") != 0)
		{
			function->setisLeftClick(false);
			function->mouseUD(drawPInfo, FALSE);
			function->draw(WndFunc::canvasWnd, drawPInfo, FALSE);
			break;
		}

		if (IsWindowVisible(WndFunc::fileManager) || IsWindowVisible(WndFunc::sideWnd))
		{
			function->setisLeftClick(false);
			ShowWindow(WndFunc::fileManager, SW_HIDE); // ���� ������ �ݱ�
			ShowWindow(WndFunc::sideWnd, SW_HIDE); // ���� ������ �ݱ�
			DW_NameBar::sideMenu.toggleState = false;
			InvalidateRect(WndFunc::nameWnd, NULL, TRUE);
			break;
		}

		/// ĵ�������� �׸� �� ���� â ���������� ����
		if (IsWindowVisible(WndFunc::colorWnd))
		{
			ShowWindow(WndFunc::colorWnd, SW_HIDE); // ���� ������ �ݱ�
		}
		{
	case WM_LBUTTONUP:
		if (!function->getIsReset())
		{
			break;
		}
		drawPInfo.lParam = lParam;
		drawPInfo.pColor = DW_ColorBox::getColorNum(DW_ColorBox::colorSelect);//ColorPalette::colorArr[Function::penNum];
		drawPInfo.pTime = (DWORD)GetTickCount64();
		drawPInfo.pWidth = penThickness->getPenWidth(); /// �� ���� ����
		drawPInfo.state = message;
		function->mouseUD(drawPInfo, TRUE);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(WndFunc::canvasWnd, &ps);

		function->paint(memDC, WndFunc::wndSize, ps);

		//memDC �� hdc�� ��� ����
		BitBlt(hdc, 0, 0, WndFunc::wndSize.right, WndFunc::wndSize.bottom, memDC, 0, 0, SRCCOPY);

		ReleaseDC(hWnd, hdc);
		EndPaint(WndFunc::canvasWnd, &ps);
		break;
	}

	case WM_DESTROY:
	{
		DeleteObject(memDC);
		DeleteObject(hBitmap);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}
