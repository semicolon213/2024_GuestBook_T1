#include "DW_ToolMenu.h"
#include "PenThickness.h"

/// 펜 종류 버튼 생성
MakeButton basicPenButton(10, 10, 40, 40);
MakeButton pencilButton(50, 10, 80, 40);
MakeButton brushButton(90, 10, 120, 40);
MakeButton sprayButton(130, 10, 160, 40);
MakeButton rectpenButton(170, 10, 200, 40);
MakeButton waterpenButton(210, 10, 240, 40);

/// 색상 종류 버튼 생성
MakeButton colorButton1;
MakeButton colorButton2;
MakeButton colorButton3;


MakeButton eraseButton;		/// 지우개 버튼 생성
MakeButton playButton;		/// 플레이 버튼 생성
MakeButton stopButton;		/// 중지 버튼 생성
MakeButton saveButton;		/// 세이브 버튼 생성

ColorPalette colorGet;		/// 색상 받아오기용 인스턴스 

PAINTSTRUCT t_ps = { 0 };
HBRUSH ToolBrush = nullptr;
HPEN ToolPen = nullptr;
HDC tHdc = nullptr;

HPEN tt1, tt2;
HBRUSH tb1, tb2;

RECT a;			/// IntersectRect 반환용 RECT
RECT mouse;		/// 마우스 좌표 기준 RECT 생성

/// LBUTTONDOWN시 마다 selectedButton과 selectedIcon이 업데이트
/// 이펙트 처리는 WM_PAINT에서 처리
/// 클릭시 selectedButton 변수에 해당 RECT 데이터 저장
/// buttonLoadImage 메서드로 해당 Rect에 이펙트 생성
MakeButton* selectedButton = nullptr;  // 현재 선택된 버튼
int selectedIcon = 0;                  // 현재 선택된 아이콘 ID

/// 클릭한 색상 추적용 변수
MakeButton* selectedColorButton = nullptr;  // 현재 선택된 색상 버튼


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

	int midPoint = toolRT.right / 2;

	roundRgn = CreateEllipticRgn(0, 0, 30, 30);

	eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
	playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
	stopButton.setCoordinate(midPoint + 160, 10, midPoint+190, 40);
	saveButton.setCoordinate(toolRT.right - 50, 10, toolRT.right - 20, 40);
}

LRESULT DW_ToolMenu::HandleMessage(HWND tWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
		toolRT = ChildWindow::GetRT();
	case WM_CREATE:
	{
		function = make_unique<Function>();
		colorPalette = make_unique<ColorPalette>();

		/// 프로그램 실행 시 기본 펜(BASIC) 설정
		selectedButton = &basicPenButton;  // 기본 펜 버튼 설정
		selectedIcon = IDI_PEN_ICON;       // 기본 펜 아이콘 설정
		/// 프로그램 실행 시 컬러 버튼1 기본값
		selectedColorButton = &colorButton1;
		break;
	}
	/// 창 사이즈 변경시 마다 이미지 아이콘들 위치 조정
	case WM_SIZE:
	{
		toolRT = ChildWindow::GetRT();  // 창의 새로운 크기 가져오기
		int midPoint = toolRT.right / 2;  // 중앙 좌표 계산

		eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
		playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
		stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);
		saveButton.setCoordinate(toolRT.right - 50, 10, toolRT.right - 20, 40);

		break;
	}
	case WM_LBUTTONDOWN:
	{
		HDC hdc = GetDC(tWnd);

		/// 현재 마우스 좌표로 사각형 생성
		mouse.left = LOWORD(lParam);
		mouse.top = HIWORD(lParam);
		mouse.right = mouse.left + 1;
		mouse.bottom = mouse.top + 1;

		/// 기본 펜
		if (IntersectRect(&a, &mouse, &basicPenButton.rectButton)) {
			function->setBShape(BASIC);
			/// 선택된 버튼 기록용 변수에 현재 객체 저장
			selectedButton = &basicPenButton;
			/// 선택된 아이콘 값 저장
			selectedIcon = IDI_PEN_ICON;
		}
		/// 펜슬
		else if (IntersectRect(&a, &mouse, &pencilButton.rectButton)) {
			function->setBShape(PENCIL);
			selectedButton = &pencilButton;
			selectedIcon = IDI_PENCIL_ICON;
		}
		/// 브러시
		else if (IntersectRect(&a, &mouse, &brushButton.rectButton)) {
			function->setBShape(BRUSH);

			selectedButton = &brushButton;
			selectedIcon = IDI_BRUSH_ICON;
		}
		/// 스프레이
		else if (IntersectRect(&a, &mouse, &sprayButton.rectButton)) {
			function->setBShape(SPRAY);

			selectedButton = &sprayButton;
			selectedIcon = IDI_SPRAY_ICON;
		}
		/// 네모펜
		else if (IntersectRect(&a, &mouse, &rectpenButton.rectButton)) {
			function->setBShape(MARKER);
			selectedButton = &rectpenButton;
			selectedIcon = IDI_RECTPEN_ICON;
			
		}
		/// 물펜
		else if (IntersectRect(&a, &mouse, &waterpenButton.rectButton)) {
			function->setBShape(WATERCOLOR);

			selectedButton = &waterpenButton;
			selectedIcon = IDI_WATERPEN_ICON;
		}

		/// 색상 버튼 1
		if (IntersectRect(&a, &mouse, &colorButton1.rectButton)) {
			if (Function::penNum == 0) { colorPalette->colorSelect(tWnd, 0); }
			else { Function::penNum = 0; }

			selectedColorButton = &colorButton1;	/// 선택한 컬러버튼의 객체 저장
		}
		/// 색상 버튼 2
		if (IntersectRect(&a, &mouse, &colorButton2.rectButton)) {
			if (Function::penNum == 1) { colorPalette->colorSelect(tWnd, 1); }
			else { Function::penNum = 1; }

			selectedColorButton = &colorButton2;	
		}
		/// 색상 버튼 3
		if (IntersectRect(&a, &mouse, &colorButton3.rectButton)) {
			if (Function::penNum == 2) { colorPalette->colorSelect(tWnd, 2); }
			else { Function::penNum = 2; }

			selectedColorButton = &colorButton3;	
		}

		/// 지우개 버튼 
		if (IntersectRect(&a, &mouse, &eraseButton.rectButton)) {

			if (function->getDrawLInfoEmpty()) { break; }
			if (!function->getIsReplay()) {
				SendMessage(Function::hWnd, WM_COMMAND, TL_CLEAR_BT, 0);
			}
		}

		/// 리플레이 버튼
		if (IntersectRect(&a, &mouse, &playButton.rectButton)) {
			if (function->getDrawLInfoEmpty()) { break; }

			playButton.toggleState = !playButton.toggleState;

			if (pCnt)
			{
				if (!function->getIsReplay())
				{
					/// 중심 좌표 계산
					int midPoint = toolRT.right / 2;

					SendMessage(Function::hWnd, WM_COMMAND, TL_PLAY_BT, 0);

					/// 다른 버튼들 안보이게 설정
					saveButton.setCoordinate(-50, -50, -50, -50);
					eraseButton.setCoordinate(-50, -50, -50, -50);
					colorButton1.setCoordinate(-50, -50, -50, -50);
					colorButton2.setCoordinate(-50, -50, -50, -50);
					colorButton3.setCoordinate(-50, -50, -50, -50);

					/// 재생, 중지 버튼 위치 조정
					playButton.setCoordinate(midPoint - 50, 10, midPoint - 20, 40);
					stopButton.setCoordinate(midPoint + 20, 10, midPoint + 50, 40);

					pCnt = false;
					*tCnt = false;	

					InvalidateRect(tWnd, NULL, true);
				}
			}
			else
			{
				SendMessage(Function::hWnd, WM_COMMAND, TL_PLAY_BT, 1);
				pCnt = true;
			}

			InvalidateRect(tWnd, NULL, true);
			UpdateWindow(tWnd);
		}

		/// 중지 버튼
		if (IntersectRect(&a, &mouse, &stopButton.rectButton)) {

			if (function->getIsReplay()) {
				SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 0);
			}
			else {
				SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 1);

				toolRT = ChildWindow::GetRT();  // 창의 새로운 크기 가져오기

				int midPoint = toolRT.right / 2;	/// 윈도우 중심 좌표 계산

				/// 정지버튼 클릭 즉, 리플레이 중지시 지우개, 저장 버튼 원래 자리로 복귀
				eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);

				playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
				stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);
				saveButton.setCoordinate(toolRT.right - 50, 10, toolRT.right - 20, 40);

				playButton.toggleState = false;
			}

			pCnt = true;
			*tCnt = true;

			InvalidateRect(tWnd, NULL, true);
		}

		/// 저장 버튼 
		if (IntersectRect(&a, &mouse, &saveButton.rectButton)) {
			MessageBox(tWnd, L"저장 구현중", L"SOS", MB_OK);
		}
		InvalidateRect(tWnd, NULL, true);  // 화면 갱신
		ReleaseDC(tWnd, hdc);
		break;
	}
	case WM_LBUTTONUP:
	{

	}
	/// 툴 메뉴 바의 PAINT 영역
	case WM_PAINT:
	{
		toolRT = ChildWindow::GetRT();

		// 더블 버퍼링을 위한 메모리 DC 생성
		HDC hdc = BeginPaint(tWnd, &t_ps);
		HDC memDC = CreateCompatibleDC(hdc);  // 메모리 DC 생성
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, toolRT.right - toolRT.left, toolRT.bottom - toolRT.top);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

		/// 배경 흰색(RGB(255,255,255))
		HBRUSH backgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(memDC, &toolRT, backgroundBrush);
		DeleteObject(backgroundBrush);

		/// 중심 좌표 계산
		int midPoint = toolRT.right / 2;

		/// 버튼과 이미지 그리기
		basicPenButton.drawRectButton(memDC, IDI_PEN_ICON);
		pencilButton.drawRectButton(memDC, IDI_PENCIL_ICON);
		brushButton.drawRectButton(memDC, IDI_BRUSH_ICON);
		sprayButton.drawRectButton(memDC, IDI_SPRAY_ICON);
		rectpenButton.drawRectButton(memDC, IDI_RECTPEN_ICON);
		waterpenButton.drawRectButton(memDC, IDI_WATERPEN_ICON);

		eraseButton.drawRectButton(memDC, IDI_ERASE_ICON);

		playButton.doubleImgButton(memDC, IDI_PAUSE_ICON, IDI_PLAY_ICON);
		stopButton.drawRectButton(memDC, IDI_STOP_ICON);
		saveButton.drawRectButton(memDC, IDI_SAVE_ICON);

		/// 선택된 브러시 버튼에 이펙트 적용
		if (selectedButton != nullptr) {
			selectedButton->clickEffectPen(IDI_PENEFFECT_ICON, selectedIcon, memDC);
		}

		/// 색상 버튼 그리기
		/// tCnt 변수가 참일때만 그림 
		/// 컬러 버튼은 Ellipse로 구현하였기에 WM_PAINT에서 좌표 처리
		if (*tCnt)
		{
			colorButton1.setCoordinate(midPoint - 140, 10, midPoint - 110, 40);
			colorButton2.setCoordinate(midPoint - 90, 10, midPoint - 60, 40);
			colorButton3.setCoordinate(midPoint - 40, 10, midPoint - 10, 40);

			/// 선택된 컬러 버튼에 이펙트 적용
			if (selectedColorButton != nullptr) {
				selectedColorButton->clickEffectPen(IDI_COLOREFFECT_ICON, memDC);
			}

			/// 색상 버튼 1 미리보기
			tb1 = CreateSolidBrush(colorGet.getColor(0));
			tb2 = (HBRUSH)SelectObject(memDC, tb1);
			colorButton1.drawEllipseButton(memDC);
			SelectObject(memDC, tb2);
			DeleteObject(tb1);

			// 색상 버튼 2 미리보기
			tb1 = CreateSolidBrush(colorGet.getColor(1));
			tb2 = (HBRUSH)SelectObject(memDC, tb1);
			colorButton2.drawEllipseButton(memDC);
			SelectObject(memDC, tb2);
			DeleteObject(tb1);

			// 색상 버튼 3 미리보기
			tb1 = CreateSolidBrush(colorGet.getColor(2));
			tb2 = (HBRUSH)SelectObject(memDC, tb1);
			colorButton3.drawEllipseButton(memDC);

			SelectObject(memDC, tb2);
			DeleteObject(tb1);

			MoveToEx(memDC, midPoint + 98, 8, nullptr);
			LineTo(memDC, midPoint + 98, 42);


		}
		// 메모리 DC에서 실제 DC로 복사 (더블 버퍼링 적용)
		BitBlt(hdc, 0, 0, toolRT.right - toolRT.left, toolRT.bottom - toolRT.top, memDC, 0, 0, SRCCOPY);

		// 리소스 정리
		SelectObject(memDC, oldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(memDC);
		EndPaint(tWnd, &t_ps);

		break;
	}
	default:
		return ChildWindow::HandleMessage(tWnd, message, wParam, lParam);
	}
	return 0;
}