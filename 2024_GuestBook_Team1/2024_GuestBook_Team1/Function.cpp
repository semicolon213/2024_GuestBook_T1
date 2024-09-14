/**
@author 조재현
@date 2024.09.15
	브러쉬 종류 기능 업뎃
@todo 그림판 처럼 정제된 브러쉬 기능 연구중.	  
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


void Function::draw(HWND hWnd, LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)// 뒤에 브러쉬 추가
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

		//std::thread를 사용하여 스레드를 시작
		replayThreadHandle = thread(&Function::replay, this, hWnd);

	//스레드가 종료될 때 자동으로 자원이 반환되도록 함
	replayThreadHandle.detach();
}

void Function::replay(HWND hWnd)
{
	HDC hdc;

	while (!isTerminate)
	{
		//화면 초기화
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

			//재생 속도 조절
			if (i < drawLInfo.pInfo.size() - 1)
			{
				Sleep(drawLInfo.pInfo[i + 1].pTime - drawLInfo.pInfo[i].pTime);
			}

			DeleteObject(nPen);
		}

		ReleaseDC(hWnd, hdc);

		//반복 간격 조절
		Sleep(500);
	}
}

void Function::setPenStyle(int size, LPARAM lParam, COLORREF col)
{
	x = LOWORD(lParam);
	y = HIWORD(lParam);

	// 브러쉬 선택하면 거기에 맞는 펜 제공
	switch (bShape)
	{
	case BASIC: // 기본 그리기
		nPen = CreatePen(PS_SOLID, size, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;
	case PENCIL: // 연필 (색깔,크기 만 조정)		
		nPen = CreatePen(PS_SOLID, 2, RGB(130,130,130));
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;
	case SPRAY: // 스프레이 (점을 흩뿌림)
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
		int alpha = 10; // 기본 투명도 설정    		
		PointF points[50]; // 도형 꼭짓점 갯수
		for (int i = 0; i < 50; ++i) {			
			float angle = 2 * 3.14159f * i / 50; // 꼭짓점 좌표 

			points[i] = PointF(x + size * cos(angle), y + size * sin(angle)); // 꼭짓점 설정
		}		
		SolidBrush brush(Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col)));	// 색상 설정
		graphics.FillPolygon(&brush, points, 50); // 정형화 되지 않는 도형 그리기	
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
void Function::GDIPlusEnd() { //gdi 종료
	GdiplusShutdown(gdiplusToken);
}
