/**
* @author ������
* @date 2024.09.25
* @detailes 
	�귯�� ���� ��� ����
	using namespace Gdiplus ���� �� ����
	Resource.h / �귯�� ���� ���� �̸� ����
	Function.h / setPenStyle �μ� ����, ���� bshape �� ����
	DW_Canvas.cpp / GDI+ ����� ����ϱ� ���� ���� �ڵ� �� �� �߰�
* @todo �� �귯�� ��� �ٸ� ���Ͽ��� ���� ��
*@	
**/
#include "Function.h"

using namespace std;

void Function::record(PINFO inputPI)
{
	inputPI.bShape = bShape;
	drawLInfo.pInfo.push_back(inputPI);
}

// �⺻ �׸��� ��ɿ� �귯�� ��� �ڵ� �߰���.
void Function::draw(HWND hWnd, PINFO dInfo, bool isRecord)// �ڿ� �귯�� �߰�
{

	hdc = GetDC(hWnd);
	if (isLeftClick)
	{
		x = LOWORD(dInfo.lParam);
		y = HIWORD(dInfo.lParam);

		setPenStyle(dInfo.pWidth, dInfo, dInfo.pColor);

		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, px, py);
		DeleteObject(nPen);

		px = x;
		py = y;

		
		if (isRecord)
			record(dInfo);

	}

}

void Function::mouseUD(PINFO dInfo, bool isRecord)
{
	if (dInfo.state == WM_LBUTTONDOWN)
	{
		x = LOWORD(dInfo.lParam);
		y = HIWORD(dInfo.lParam);

		px = x;
		py = y;

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

void Function::clearDrawing(HWND hWnd) {
	// ��� ����
	drawLInfo.pInfo.clear();

	// ȭ�� �ʱ�ȭ
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
}

void Function::setPenStyle(int size, PINFO dinfo, COLORREF col)
{
	x = LOWORD(dinfo.lParam);
	y = HIWORD(dinfo.lParam);

	// �귯�� �����ϸ� �ű⿡ �´� �� ����
	switch (bShape)
	{
	case BASIC: // �⺻ �׸���
		nPen = CreatePen(PS_SOLID, size, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;
	case BRUSH:
		// ���� ��..
		break;
	case PENCIL:
	{
	Gdiplus::Graphics graphics(hdc);
	int alpha = 15; // �⺻ ���� ����    		
	Gdiplus::PointF points[80]; // ���� ������ ����
	for (int i = 0; i < 80; ++i) 
		{
		INT angle = rand() % 6 * 3.14159f * i / 80; // ������ ��ǥ 
		points[i] = Gdiplus::PointF(x + size * cos(angle) / 2, y + size * sin(angle) / 2); // ������ ����
		}
	Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col)));	// ���� ����
	graphics.FillPolygon(&brush, points, 80); // ����ȭ ���� �ʴ� ���� �׸���	
	ReleaseDC(hWnd, hdc);	
	break;
	}
	

	case SPRAY: // �������� (���� ��Ѹ�)
		for (int i = 0; i < 200; ++i)
		{
			int offsetX = (rand() % (size * 8)) - (size * 4);
			int offsetY = (rand() % (size * 8)) - (size * 4);
			if (sqrt(offsetX * offsetX + offsetY * offsetY) <= size * 2)
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
		graphics.FillRectangle(&marker, x - size, y - size, size * 2, size * 2);
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
			float radius = size + (rand() % 10); // ������ �ݰ� ��ȭ
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





void Function::setBShape(int bShape)
{
	this->bShape = bShape;
}

LINFO Function::getDrawLInfo()
{
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
