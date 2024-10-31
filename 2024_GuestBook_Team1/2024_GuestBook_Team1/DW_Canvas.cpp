#include "DW_Canvas.h"

/// 네임 바 정적 메서드
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
		return pThis->handleMessageCV(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/// 네임 바 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_CREATE:
	{
		function = std::make_unique<Function>();
		function->GDIPlusStart(); // 붓 gdi 라이브러리 활성화

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
		drawPInfo.pWidth = penThickness->getPenWidth(); /// 펜 굵기 설정
		drawPInfo.state = message;
		function->draw(hWnd, drawPInfo, TRUE); // 브러쉬 기능 추가하려면 해당 RECTANGLE 에 알맞는 변수를 넣으면 됨.

		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		if (!function->getIsReset()) break;
		drawPInfo.lParam = lParam;
		drawPInfo.pColor = RGB(0, 0, 0);//ColorPalette::colorArr[Function::penNum];
		drawPInfo.pTime = (DWORD)GetTickCount64();
		drawPInfo.pWidth = penThickness->getPenWidth(); /// 펜 굵기 설정
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


