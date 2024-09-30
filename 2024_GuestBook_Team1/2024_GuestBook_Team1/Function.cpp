/**
@author ������
@date 2024.09.28
	�� �귯�� ��� �߰�
	draw �Լ� ���� ����
	mouseUD �Լ� ���� ����	
**/
#include "Function.h"


using namespace std;


int Function::penNum = 0;
LINFO Function::drawLInfo = { };
HWND Function::hWnd = nullptr;
int Function::bShape = BASIC;

void Function::record(PINFO inputPI)
{

	inputPI.bShape = bShape;
	drawLInfo.pInfo.push_back(inputPI);

	/*std::wstring message = L"record() ȣ���, drawLInfo.pInfo ũ��: " + std::to_wstring(drawLInfo.pInfo.size()) +
		L"\nFunction ��ü �ּ�: " + std::to_wstring((uintptr_t)this);
	MessageBox(nullptr, message.c_str(), L"�����: record", MB_OK); */

	

}

void Function::draw(HWND hWnd, PINFO dInfo, bool isRecord) // �ڿ� �귯�� �߰�
{

	hdc = GetDC(hWnd);
	if (isLeftClick)
	{		
		px = LOWORD(dInfo.lParam); // �׸��� ������ ��ǥ
		py = HIWORD(dInfo.lParam);

		currentTime = std::chrono::steady_clock::now(); // �׸��� �ð� ����

		setPenStyle(dInfo, dInfo.pColor);
		
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, px, py);
		DeleteObject(nPen);
				
		x = px; 
		y = py;
		
		DrawTime = currentTime; // ������ �ð� ������Ʈ

		if (isRecord)
			record(dInfo);

	}
	ReleaseDC(hWnd, hdc);

}

void Function::mouseUD(PINFO dInfo, bool isRecord)
{
	if (dInfo.state == WM_LBUTTONDOWN)
	{
		x = LOWORD(dInfo.lParam); //��Ŭ�� �� ��ǥ
		y = HIWORD(dInfo.lParam);

		DrawTime = std::chrono::steady_clock::now(); // �� �귯�� ����� ���� �׸��� ���� �ð� ����.		
		lastThicknessChangeTime = DrawTime; // �β� ���� �ð� �ʱ�ȭ
		currentThickness = dInfo.pWidth; // �� ���� �� ������.


		isLeftClick = true;
	} else
	{
		isLeftClick = false;
	}

	
	if (isRecord)
		record(dInfo);
	
}

void Function::replayThread(HWND hWnd)
{
	setIsReplay(true);

	if (replayThreadHandle.joinable())
		return;
	else
		// std::thread�� ����Ͽ� ������ ����
		replayThreadHandle = thread(&Function::replay, this, hWnd);

	// �����尡 ����� �� �ڵ����� �ڿ��� ��ȯ�ǵ��� ��
	replayThreadHandle.detach();
}

// �⺻ ���÷��� ���� �Լ�
void Function::replay(HWND hWnd)
{
	HDC hdc;

	while (isReplay)
	{
		// ȭ�� �ʱ�ȭ
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);

		hdc = GetDC(hWnd);

		for (size_t i = 0; i < drawLInfo.pInfo.size(); i++)
		{
			if (!isReplay)
			{
				isLeftClick = false;
				break;
			}

			PINFO replayInfo = drawLInfo.pInfo[i];
			x = LOWORD(replayInfo.lParam);
			y = HIWORD(replayInfo.lParam);

			setBShape(replayInfo.bShape);

			switch (replayInfo.state)
			{
			case WM_LBUTTONDOWN:
				mouseUD(replayInfo, false);
				break;

			case WM_MOUSEMOVE:
				draw(hWnd, replayInfo, false);
				break;

			case WM_LBUTTONUP:
				mouseUD(replayInfo, false);
				break;

			default:
				break;

			}

			// ��� �ӵ� ����
			if (i < drawLInfo.pInfo.size() - 1)
			{
				Sleep(drawLInfo.pInfo[i + 1].pTime - drawLInfo.pInfo[i].pTime);
			}

			DeleteObject(nPen);
		}


		ReleaseDC(hWnd, hdc);

		// �ݺ� ���� ����	
		Sleep(500);
	}
}

// RESET ��ư Ŭ�� �� �۵��Ǵ� �Լ� (���� ���·� ����)
void Function::reDrawing(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	for (const auto& replayInfo : drawLInfo.pInfo)
	{
		x = LOWORD(replayInfo.lParam);
		y = HIWORD(replayInfo.lParam);

		setBShape(replayInfo.bShape);

		switch (replayInfo.state)
		{
		case WM_LBUTTONDOWN:
			mouseUD(replayInfo, false);
			break;

		case WM_MOUSEMOVE:
			draw(hWnd, replayInfo, false);
			break;

		case WM_LBUTTONUP:
			mouseUD(replayInfo, false);
			break;

		default:
			break;
		}
	}

	ReleaseDC(hWnd, hdc);
}

void Function::clearDrawing(HWND hWnd)
{
	// ��� ����
	drawLInfo.pInfo.clear();

	// ȭ�� �ʱ�ȭ
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);		
}
void Function::setPenStyle(PINFO dinfo, COLORREF col)
{
	// �귯�� �����ϸ� �ű⿡ �´� �� ����
	switch (bShape)
	{
	case BASIC: // �⺻ �׸���
		nPen = CreatePen(PS_SOLID, dinfo.pWidth, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;

	case BRUSH: // �� �귯��
	{
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - DrawTime).count(); //������ �� �ð����� �׷��� �� �ð� �� ������ �ð�(�и���)
		duration = max(duration, 1); // �ּ� duration ���� �����Ͽ� 0���� ������ ���� ����

		double distance = sqrt(pow(px - x, 2) + pow(py - y, 2)); // �� �Ÿ�
		double speed = (distance / duration) * 1000; // �ӵ� ���

		int targetThickness = dinfo.pWidth; // �ӵ��� ����� �� ���� ���� �Ǵ� �β� ����

		// �ӵ��� ���� �� �β� ���̱�
		if (speed > Threshold_Speed) {
			targetThickness = dinfo.pWidth - (int)((speed - Threshold_Speed) / (Threshold_Speed / (dinfo.pWidth - Min_Thickness)));
			targetThickness = max(targetThickness, Min_Thickness);
		}
		// �ӵ��� ���� �� �β� �ø���
		else {
			targetThickness = Min_Thickness + (int)((Threshold_Speed - speed) / (Threshold_Speed / (dinfo.pWidth - Min_Thickness)));
			targetThickness = min(targetThickness, dinfo.pWidth);
		}

		// �β� ��ȭ ������ �������� Ȯ��		
		if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastThicknessChangeTime).count() >= Update_Interval)
		{
			// �� ���� ������ ����
			if (currentThickness < targetThickness)
				currentThickness += Smoothing_Factor;
			else if (currentThickness > targetThickness)
				currentThickness -= Smoothing_Factor;

			// �β� ���� �ð� ������Ʈ
			lastThicknessChangeTime = currentTime; 		
		}		
		// �β��� ����� �� ����
		nPen = CreatePen(PS_SOLID, currentThickness, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;
	}

	case PENCIL:
	{
		Gdiplus::Graphics graphics(hdc);
		int alpha = 15; // �⺻ ���� ����    		
		Gdiplus::PointF points[80]; // ���� ������ ����
		for (int i = 0; i < 80; ++i)
		{
			INT angle = rand() % 6 * 3.14159f * i / 80; // ������ ��ǥ 
			points[i] = Gdiplus::PointF(x + dinfo.pWidth * cos(angle) / 2, y + dinfo.pWidth * sin(angle) / 2); // ������ ����
		}
		Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col)));	// ���� ����
		graphics.FillPolygon(&brush, points, 80); // ����ȭ ���� �ʴ� ���� �׸���	
		ReleaseDC(hWnd, hdc);
		break;
	}

	case SPRAY: // �������� (���� ��Ѹ�)
		for (int i = 0; i < 500; ++i)
		{
			int offsetX = (rand() % (dinfo.pWidth * 8)) - (dinfo.pWidth * 4);
			int offsetY = (rand() % (dinfo.pWidth * 8)) - (dinfo.pWidth * 4);
			if (sqrt(offsetX * offsetX + offsetY * offsetY) <= dinfo.pWidth * 2)
			{
				SetPixel(hdc, x + offsetX, y + offsetY, col);
			}
		}
		ReleaseDC(hWnd, hdc);
		break;

	case MARKER:
	{
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::SolidBrush marker(Gdiplus::Color(40, GetRValue(col), GetGValue(col), GetBValue(col)));
		graphics.FillRectangle(&marker, x - dinfo.pWidth, y - dinfo.pWidth, dinfo.pWidth * 2, dinfo.pWidth * 2);
		ReleaseDC(hWnd, hdc);
		break;
	}

	case WATERCOLOR:
	{
		Gdiplus::Graphics graphics(hdc);
		int alpha = 10; // �⺻ ���� ����
		const int numPoints = 20; // ������ ����
		Gdiplus::PointF points[numPoints];

		// ������ ������ ����Ͽ� ���������� ����� �����
		for (int i = 0; i < numPoints; ++i) {
			float angle = 2 * 3.14159f * i / numPoints; // ���� ��ǥ
			float radius = dinfo.pWidth + (rand() % 10); // ������ �ݰ� ��ȭ
			points[i] = Gdiplus::PointF(x + radius * cos(angle), y + radius * sin(angle)); // ������ ����
		}

		Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col))); // ���� ����
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias); // �ε巴�� �׸���
		graphics.FillPolygon(&brush, points, numPoints); // ������ ���� �׸���

		ReleaseDC(hWnd, hdc);
		break;
	}

	default:
		break;
	}
}

void Function::paint(HWND hWnd, RECT canvasRT)
{
	cHdc = BeginPaint(hWnd, &cPS);
	CanvasPen = (HPEN)SelectObject(cHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
	Rectangle(cHdc, canvasRT.left, canvasRT.top, canvasRT.right, canvasRT.bottom);
	SelectObject(cHdc, CanvasPen);
	DeleteObject(CanvasPen);

	if (!getIsReplay())
	{
		for (const auto& record : getDrawLInfo().pInfo)
		{
			setBShape(record.bShape);

			switch (record.state)
			{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
				mouseUD(record, FALSE);
				break;

			case WM_MOUSEMOVE:
				draw(hWnd, record, FALSE);
				break;

			default:
				break;
			}
		}
	}

	EndPaint(hWnd, &cPS);
}




void Function::setBShape(int bShape)
{
	this->bShape = bShape;
}

LINFO Function::getDrawLInfo()
{
	/*std::wstring message = L"getDrawLInfo() ȣ��, drawLInfo.pInfo ũ��: " + std::to_wstring(drawLInfo.pInfo.size()) +
		L"\nFunction ��ü �ּ�: " + std::to_wstring((uintptr_t)this);
	MessageBox(nullptr, message.c_str(), L"�����: getDrawLInfo", MB_OK); */
	return drawLInfo;
}

void Function::setIsReplay(bool isReplay)
{
	this->isReplay = isReplay;
}

bool Function::getIsReplay() 
{
	return isReplay;
}

// RESET ��ư Ŭ�� �� ȣ��Ǵ� �Լ�
void Function::stopReplay(HWND hWnd)
{
	setIsReplay(false);
	reDrawing(hWnd);
}

// ���Ͱ� ����ִ��� �˻�
bool Function::getDrawLInfoEmpty()
{
	return drawLInfo.pInfo.empty();
}
void Function::GDIPlusStart()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
void Function::GDIPlusEnd() { //gdi ����
	Gdiplus::GdiplusShutdown(gdiplusToken);
}
