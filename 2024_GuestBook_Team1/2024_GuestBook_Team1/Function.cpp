/**
@author ������
@date 2024.09.15
	�귯�� ���� ��� ����
@todo �׸��� ó�� ������ �귯�� ��� ������.	  
**/
#include "Function.h"

using namespace std;
using namespace Gdiplus;

ULONG_PTR gdiplusToken;

void Function::record(LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)
{
	drawPInfo.lParam = lParam;
	drawPInfo.pTime = pTime;
	drawPInfo.state = state;
	drawPInfo.pWidth = size;
	drawPInfo.pColor = col;
	drawPInfo.bShape = bShape;
	drawLInfo.pInfo.push_back(drawPInfo);
}


void Function::draw(HWND hWnd, LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)// �ڿ� �귯�� �߰�
{

	hdc = GetDC(hWnd);
	if (isLeftClick)
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		setPenStyle(size, lParam, col);

		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, px, py);
		DeleteObject(nPen);


		px = x;
		py = y;

		if (isReplay) return;

		record(lParam, pTime, state, size, col);

	}

}

void Function::mouseUD(LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)
{
	if (state == WM_LBUTTONDOWN)
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		px = x;
		py = y;

		isLeftClick = true;
	}
	else
	{
		isLeftClick = false;
	}

	if (isReplay) return;

	record(lParam, pTime, state, size, col);
}

void Function::replayThread(HWND hWnd)
{
	isTerminate = false;
	setIsReplay(true);

	if (replayThreadHandle.joinable())
		return;

	else

		//std::thread�� ����Ͽ� �����带 ����
		replayThreadHandle = thread(&Function::replay, this, hWnd);

	//�����尡 ����� �� �ڵ����� �ڿ��� ��ȯ�ǵ��� ��
	replayThreadHandle.detach();
}

void Function::replay(HWND hWnd)
{
	HDC hdc;

	while (!isTerminate)
	{
		//ȭ�� �ʱ�ȭ
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);

		hdc = GetDC(hWnd);

		for (size_t i = 0; i < drawLInfo.pInfo.size(); i++)
		{
			if (isTerminate)
				break;

			PINFO replayInfo = drawLInfo.pInfo[i];
			x = LOWORD(replayInfo.lParam);
			y = HIWORD(replayInfo.lParam);

			switch (replayInfo.state)
			{
			case WM_LBUTTONDOWN:
				setBShape(replayInfo.bShape);
				mouseUD(replayInfo.lParam, replayInfo.pTime, replayInfo.state, replayInfo.pWidth, replayInfo.pColor);
				break;

			case WM_MOUSEMOVE:
				setBShape(replayInfo.bShape);
				draw(hWnd, replayInfo.lParam, replayInfo.pTime, replayInfo.state, replayInfo.pWidth, replayInfo.pColor);
				break;

			case WM_LBUTTONUP:
				setBShape(replayInfo.bShape);
				mouseUD(replayInfo.lParam, replayInfo.pTime, replayInfo.state, replayInfo.pWidth, replayInfo.pColor);

				break;

			default:
				break;
			}

			//��� �ӵ� ����
			if (i < drawLInfo.pInfo.size() - 1)
			{
				Sleep(drawLInfo.pInfo[i + 1].pTime - drawLInfo.pInfo[i].pTime);
			}

			DeleteObject(nPen);
		}

		ReleaseDC(hWnd, hdc);

		//�ݺ� ���� ����
		Sleep(500);
	}
}

void Function::setPenStyle(int size, LPARAM lParam, COLORREF col)
{
	x = LOWORD(lParam);
	y = HIWORD(lParam);

	// �귯�� �����ϸ� �ű⿡ �´� �� ����
	switch (bShape)
	{
	case BASIC: // �⺻ �׸���
		nPen = CreatePen(PS_SOLID, size, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;
	case PENCIL: // ���� (����,ũ�� �� ����)		
		nPen = CreatePen(PS_SOLID, 2, RGB(130,130,130));
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;
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
		Graphics graphics(hdc);
		SolidBrush marker(Color(40, GetRValue(col), GetGValue(col), GetBValue(col)));
		graphics.FillRectangle(&marker, x - size, y - size, size * 2, size * 2);
		ReleaseDC(hWnd, hdc);		
		break;
	}
	case WATERCOLOR:
	{
		Graphics graphics(hdc);		
		int alpha = 10; // �⺻ ���� ����    		
		PointF points[50]; // ���� ������ ����
		for (int i = 0; i < 50; ++i) {			
			float angle = 2 * 3.14159f * i / 50; // ������ ��ǥ 

			points[i] = PointF(x + size * cos(angle), y + size * sin(angle)); // ������ ����
		}		
		SolidBrush brush(Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col)));	// ���� ����
		graphics.FillPolygon(&brush, points, 50); // ����ȭ ���� �ʴ� ���� �׸���	
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

void Function::setIsTerminate(bool isTerminate)
{
	this->isTerminate = isTerminate;
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
void Function::GDIPlusStart()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
void Function::GDIPlusEnd() { //gdi ����
	GdiplusShutdown(gdiplusToken);
}
