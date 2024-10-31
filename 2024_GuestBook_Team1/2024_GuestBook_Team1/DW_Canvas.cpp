#include "DW_Canvas.h"

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

/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_CREATE:
	{
		function = std::make_unique<Function>();
		function->GDIPlusStart(); // �� gdi ���̺귯�� Ȱ��ȭ

		break;
	}

	case WM_COMMAND:
	{
		if (wParam == TL_CLEAR_BT && lParam == 0)
		{
			if (function->getIsReset())
			{
				function->clearDrawing(hWnd);
			}

		}

		if (wParam == TL_PLAY_BT && lParam == 0)
		{
			if (!function->getIsReplay())
			{
				function->replayThread(hWnd);
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
			function->reDrawing(hWnd);
		}

	}
	break;

	case WM_MOUSEMOVE:
	{

		if (!function->getIsReset()) break;
		//hdc = GetDC(canWnd);

		////////////////////////////////////////////////

		drawPInfo.lParam = lParam;
		drawPInfo.pColor = RGB(0, 0, 0);//ColorPalette::colorArr[Function::penNum];
		drawPInfo.pTime = (DWORD)GetTickCount64();
		drawPInfo.pWidth = penThickness->getPenWidth(); /// �� ���� ����
		drawPInfo.state = message;
		function->draw(hWnd, drawPInfo, TRUE); // �귯�� ��� �߰��Ϸ��� �ش� RECTANGLE �� �˸´� ������ ������ ��.

		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		if (!function->getIsReset()) break;
		drawPInfo.lParam = lParam;
		drawPInfo.pColor = RGB(0, 0, 0);//ColorPalette::colorArr[Function::penNum];
		drawPInfo.pTime = (DWORD)GetTickCount64();
		drawPInfo.pWidth = penThickness->getPenWidth(); /// �� ���� ����
		drawPInfo.state = message;
		function->mouseUD(drawPInfo, TRUE);

		break;

	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(WndFunc::canvasWnd, &ps);

		EndPaint(WndFunc::canvasWnd, &ps);
		break;
	}


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


