/**
* @author 조재현
* @date 2024.09.25
* @detailes 
	브러쉬 종류 기능 업뎃
	using namespace Gdiplus 삭제 및 수정
	Resource.h / 브러쉬 종류 변수 이름 설정
	Function.h / setPenStyle 인수 설정, 변수 bshape 값 설정
	DW_Canvas.cpp / GDI+ 기능을 사용하기 위한 시작 코드 한 줄 추가
* @todo 붓 브러쉬 기능 다른 파일에서 구현 중
*@	
**/
#include "Function.h"

using namespace std;

void Function::record(PINFO inputPI)
{
	inputPI.bShape = bShape;
	drawLInfo.pInfo.push_back(inputPI);
}

// 기본 그리기 기능에 브러쉬 기능 코드 추가함.
void Function::draw(HWND hWnd, PINFO dInfo, bool isRecord)// 뒤에 브러쉬 추가
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
		// std::thread를 사용하여 스레드 시작
		replayThreadHandle = thread(&Function::replay, this, hWnd);

	// 스레드가 종료될 때 자동으로 자원이 반환되도록 함
	replayThreadHandle.detach();
}

void Function::replay(HWND hWnd)
{
	HDC hdc;

	while (isReplay)
	{
		// 화면 초기화
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

			// 재생 속도 조절
			if (i < drawLInfo.pInfo.size() - 1)
			{
				Sleep(drawLInfo.pInfo[i + 1].pTime - drawLInfo.pInfo[i].pTime);
			}

			DeleteObject(nPen);
		}

		ReleaseDC(hWnd, hdc);

		// 반복 간격 조절
		Sleep(500);
	}
}

void Function::clearDrawing(HWND hWnd) {
	// 기록 삭제
	drawLInfo.pInfo.clear();

	// 화면 초기화
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
}

void Function::setPenStyle(int size, PINFO dinfo, COLORREF col)
{
	x = LOWORD(dinfo.lParam);
	y = HIWORD(dinfo.lParam);

	// 브러쉬 선택하면 거기에 맞는 펜 제공
	switch (bShape)
	{
	case BASIC: // 기본 그리기
		nPen = CreatePen(PS_SOLID, size, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;
	case BRUSH:
		// 구현 중..
		break;
	case PENCIL:
	{
	Gdiplus::Graphics graphics(hdc);
	int alpha = 15; // 기본 투명도 설정    		
	Gdiplus::PointF points[80]; // 도형 꼭짓점 갯수
	for (int i = 0; i < 80; ++i) 
		{
		INT angle = rand() % 6 * 3.14159f * i / 80; // 꼭짓점 좌표 
		points[i] = Gdiplus::PointF(x + size * cos(angle) / 2, y + size * sin(angle) / 2); // 꼭짓점 설정
		}
	Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col)));	// 색상 설정
	graphics.FillPolygon(&brush, points, 80); // 정형화 되지 않는 도형 그리기	
	ReleaseDC(hWnd, hdc);	
	break;
	}
	

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
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::SolidBrush marker(Gdiplus::Color(40, GetRValue(col), GetGValue(col), GetBValue(col)));		
		graphics.FillRectangle(&marker, x - size, y - size, size * 2, size * 2);
		ReleaseDC(hWnd, hdc);		
		break;
	}
	case WATERCOLOR:
	{
		Gdiplus::Graphics graphics(hdc);
		int alpha = 10; // 기본 투명도 설정
		const int numPoints = 20; // 꼭짓점 갯수
		Gdiplus::PointF points[numPoints];

		// 무작위 각도를 사용하여 비정형적인 모양을 만들기
		for (int i = 0; i < numPoints; ++i) {
			float angle = 2 * 3.14159f * i / numPoints; // 원형 좌표
			float radius = size + (rand() % 10); // 무작위 반경 변화
			points[i] = Gdiplus::PointF(x + radius * cos(angle), y + radius * sin(angle)); // 꼭짓점 설정
		}

		Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col))); // 색상 설정
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias); // 부드럽게 그리기
		graphics.FillPolygon(&brush, points, numPoints); // 비정형 도형 그리기

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
void Function::GDIPlusEnd() { //gdi 종료
	Gdiplus::GdiplusShutdown(gdiplusToken);
}
