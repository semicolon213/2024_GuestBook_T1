#include "DW_ToolMenu.h"
#include "PenThickness.h"

/// �� ���� ��ư ����
MakeButton basicPenButton(10, 10, 40, 40);
MakeButton pencilButton(50, 10, 80, 40);
MakeButton brushButton(90, 10, 120, 40);
MakeButton sprayButton(130, 10, 160, 40);
MakeButton rectpenButton(170, 10, 200, 40);
MakeButton waterpenButton(210, 10, 240, 40);

/// ���� ���� ��ư ����
MakeButton colorButton1;
MakeButton colorButton2;
MakeButton colorButton3;


MakeButton eraseButton;		/// ���찳 ��ư ����
MakeButton playButton;		/// �÷��� ��ư ����
MakeButton stopButton;		/// ���� ��ư ����
MakeButton saveButton;		/// ���̺� ��ư ����

ColorPalette colorGet;		/// ���� �޾ƿ���� �ν��Ͻ� 

PAINTSTRUCT t_ps = { 0 };
HBRUSH ToolBrush = nullptr;
HPEN ToolPen = nullptr;
HDC tHdc = nullptr;

HPEN tt1, tt2;
HBRUSH tb1, tb2;

RECT a;			/// IntersectRect ��ȯ�� RECT
RECT mouse;		/// ���콺 ��ǥ ���� RECT ����

/// LBUTTONDOWN�� ���� selectedButton�� selectedIcon�� ������Ʈ
/// ����Ʈ ó���� WM_PAINT���� ó��
/// Ŭ���� selectedButton ������ �ش� RECT ������ ����
/// buttonLoadImage �޼���� �ش� Rect�� ����Ʈ ����
MakeButton* selectedButton = nullptr;  // ���� ���õ� ��ư
int selectedIcon = 0;                  // ���� ���õ� ������ ID

/// Ŭ���� ���� ������ ����
MakeButton* selectedColorButton = nullptr;  // ���� ���õ� ���� ��ư


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

		/// ���α׷� ���� �� �⺻ ��(BASIC) ����
		selectedButton = &basicPenButton;  // �⺻ �� ��ư ����
		selectedIcon = IDI_PEN_ICON;       // �⺻ �� ������ ����
		/// ���α׷� ���� �� �÷� ��ư1 �⺻��
		selectedColorButton = &colorButton1;
		break;
	}
	/// â ������ ����� ���� �̹��� �����ܵ� ��ġ ����
	case WM_SIZE:
	{
		toolRT = ChildWindow::GetRT();  // â�� ���ο� ũ�� ��������
		int midPoint = toolRT.right / 2;  // �߾� ��ǥ ���

		eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
		playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
		stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);
		saveButton.setCoordinate(toolRT.right - 50, 10, toolRT.right - 20, 40);

		break;
	}
	case WM_LBUTTONDOWN:
	{
		HDC hdc = GetDC(tWnd);

		/// ���� ���콺 ��ǥ�� �簢�� ����
		mouse.left = LOWORD(lParam);
		mouse.top = HIWORD(lParam);
		mouse.right = mouse.left + 1;
		mouse.bottom = mouse.top + 1;

		/// �⺻ ��
		if (IntersectRect(&a, &mouse, &basicPenButton.rectButton)) {
			function->setBShape(BASIC);
			/// ���õ� ��ư ��Ͽ� ������ ���� ��ü ����
			selectedButton = &basicPenButton;
			/// ���õ� ������ �� ����
			selectedIcon = IDI_PEN_ICON;
		}
		/// �潽
		else if (IntersectRect(&a, &mouse, &pencilButton.rectButton)) {
			function->setBShape(PENCIL);
			selectedButton = &pencilButton;
			selectedIcon = IDI_PENCIL_ICON;
		}
		/// �귯��
		else if (IntersectRect(&a, &mouse, &brushButton.rectButton)) {
			function->setBShape(BRUSH);

			selectedButton = &brushButton;
			selectedIcon = IDI_BRUSH_ICON;
		}
		/// ��������
		else if (IntersectRect(&a, &mouse, &sprayButton.rectButton)) {
			function->setBShape(SPRAY);

			selectedButton = &sprayButton;
			selectedIcon = IDI_SPRAY_ICON;
		}
		/// �׸���
		else if (IntersectRect(&a, &mouse, &rectpenButton.rectButton)) {
			function->setBShape(MARKER);
			selectedButton = &rectpenButton;
			selectedIcon = IDI_RECTPEN_ICON;
			
		}
		/// ����
		else if (IntersectRect(&a, &mouse, &waterpenButton.rectButton)) {
			function->setBShape(WATERCOLOR);

			selectedButton = &waterpenButton;
			selectedIcon = IDI_WATERPEN_ICON;
		}

		/// ���� ��ư 1
		if (IntersectRect(&a, &mouse, &colorButton1.rectButton)) {
			if (Function::penNum == 0) { colorPalette->colorSelect(tWnd, 0); }
			else { Function::penNum = 0; }

			selectedColorButton = &colorButton1;	/// ������ �÷���ư�� ��ü ����
		}
		/// ���� ��ư 2
		if (IntersectRect(&a, &mouse, &colorButton2.rectButton)) {
			if (Function::penNum == 1) { colorPalette->colorSelect(tWnd, 1); }
			else { Function::penNum = 1; }

			selectedColorButton = &colorButton2;	
		}
		/// ���� ��ư 3
		if (IntersectRect(&a, &mouse, &colorButton3.rectButton)) {
			if (Function::penNum == 2) { colorPalette->colorSelect(tWnd, 2); }
			else { Function::penNum = 2; }

			selectedColorButton = &colorButton3;	
		}

		/// ���찳 ��ư 
		if (IntersectRect(&a, &mouse, &eraseButton.rectButton)) {

			if (function->getDrawLInfoEmpty()) { break; }
			if (!function->getIsReplay()) {
				SendMessage(Function::hWnd, WM_COMMAND, TL_CLEAR_BT, 0);
			}
		}

		/// ���÷��� ��ư
		if (IntersectRect(&a, &mouse, &playButton.rectButton)) {
			if (function->getDrawLInfoEmpty()) { break; }

			playButton.toggleState = !playButton.toggleState;

			if (pCnt)
			{
				if (!function->getIsReplay())
				{
					/// �߽� ��ǥ ���
					int midPoint = toolRT.right / 2;

					SendMessage(Function::hWnd, WM_COMMAND, TL_PLAY_BT, 0);

					/// �ٸ� ��ư�� �Ⱥ��̰� ����
					saveButton.setCoordinate(-50, -50, -50, -50);
					eraseButton.setCoordinate(-50, -50, -50, -50);
					colorButton1.setCoordinate(-50, -50, -50, -50);
					colorButton2.setCoordinate(-50, -50, -50, -50);
					colorButton3.setCoordinate(-50, -50, -50, -50);

					/// ���, ���� ��ư ��ġ ����
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

		/// ���� ��ư
		if (IntersectRect(&a, &mouse, &stopButton.rectButton)) {

			if (function->getIsReplay()) {
				SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 0);
			}
			else {
				SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 1);

				toolRT = ChildWindow::GetRT();  // â�� ���ο� ũ�� ��������

				int midPoint = toolRT.right / 2;	/// ������ �߽� ��ǥ ���

				/// ������ư Ŭ�� ��, ���÷��� ������ ���찳, ���� ��ư ���� �ڸ��� ����
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

		/// ���� ��ư 
		if (IntersectRect(&a, &mouse, &saveButton.rectButton)) {
			MessageBox(tWnd, L"���� ������", L"SOS", MB_OK);
		}
		InvalidateRect(tWnd, NULL, true);  // ȭ�� ����
		ReleaseDC(tWnd, hdc);
		break;
	}
	case WM_LBUTTONUP:
	{

	}
	/// �� �޴� ���� PAINT ����
	case WM_PAINT:
	{
		toolRT = ChildWindow::GetRT();

		// ���� ���۸��� ���� �޸� DC ����
		HDC hdc = BeginPaint(tWnd, &t_ps);
		HDC memDC = CreateCompatibleDC(hdc);  // �޸� DC ����
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, toolRT.right - toolRT.left, toolRT.bottom - toolRT.top);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

		/// ��� ���(RGB(255,255,255))
		HBRUSH backgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(memDC, &toolRT, backgroundBrush);
		DeleteObject(backgroundBrush);

		/// �߽� ��ǥ ���
		int midPoint = toolRT.right / 2;

		/// ��ư�� �̹��� �׸���
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

		/// ���õ� �귯�� ��ư�� ����Ʈ ����
		if (selectedButton != nullptr) {
			selectedButton->clickEffectPen(IDI_PENEFFECT_ICON, selectedIcon, memDC);
		}

		/// ���� ��ư �׸���
		/// tCnt ������ ���϶��� �׸� 
		/// �÷� ��ư�� Ellipse�� �����Ͽ��⿡ WM_PAINT���� ��ǥ ó��
		if (*tCnt)
		{
			colorButton1.setCoordinate(midPoint - 140, 10, midPoint - 110, 40);
			colorButton2.setCoordinate(midPoint - 90, 10, midPoint - 60, 40);
			colorButton3.setCoordinate(midPoint - 40, 10, midPoint - 10, 40);

			/// ���õ� �÷� ��ư�� ����Ʈ ����
			if (selectedColorButton != nullptr) {
				selectedColorButton->clickEffectPen(IDI_COLOREFFECT_ICON, memDC);
			}

			/// ���� ��ư 1 �̸�����
			tb1 = CreateSolidBrush(colorGet.getColor(0));
			tb2 = (HBRUSH)SelectObject(memDC, tb1);
			colorButton1.drawEllipseButton(memDC);
			SelectObject(memDC, tb2);
			DeleteObject(tb1);

			// ���� ��ư 2 �̸�����
			tb1 = CreateSolidBrush(colorGet.getColor(1));
			tb2 = (HBRUSH)SelectObject(memDC, tb1);
			colorButton2.drawEllipseButton(memDC);
			SelectObject(memDC, tb2);
			DeleteObject(tb1);

			// ���� ��ư 3 �̸�����
			tb1 = CreateSolidBrush(colorGet.getColor(2));
			tb2 = (HBRUSH)SelectObject(memDC, tb1);
			colorButton3.drawEllipseButton(memDC);

			SelectObject(memDC, tb2);
			DeleteObject(tb1);

			MoveToEx(memDC, midPoint + 98, 8, nullptr);
			LineTo(memDC, midPoint + 98, 42);


		}
		// �޸� DC���� ���� DC�� ���� (���� ���۸� ����)
		BitBlt(hdc, 0, 0, toolRT.right - toolRT.left, toolRT.bottom - toolRT.top, memDC, 0, 0, SRCCOPY);

		// ���ҽ� ����
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