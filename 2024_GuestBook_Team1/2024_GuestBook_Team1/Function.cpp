/**
@author 조재현
@date 2024.09.09
	브러쉬 종류 기능 추가 (버튼 못 만들어서 코드에서 직접 수정해야 기능 사용 가능)
	버튼 추가 (버튼 만 만들고 기능 구현x)
@todo 버튼 기능 구현
	  더 다양한 브러쉬 기능 추가 생각
	  색깔,리플레이,브러쉬 크기,저장 등 다른 기능들이랑 연결점 고려하기.
**/
#include "Function.h"

using namespace std;


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

// 기본 그리기 기능에 브러쉬 기능 코드 추가함.
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

		record(lParam, pTime, state, size, col);

	}

}

void Function::mouseUD(LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)
{
	if (state == WM_LBUTTONDOWN)
	{
		isLeftClick = true;
	}
	else
	{
		isLeftClick = false;
	}

	x = LOWORD(lParam);
	y = HIWORD(lParam);

	if (isLeftClick)
	{
		setPenStyle(size, lParam, col);

		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x, y);

		px = x;
		py = y;

	}

	record(lParam, pTime, state, size, col);
}

void Function::replayThread(HWND hWnd)
{
	isTerminate = false;
	bool status = false;

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
			setBShape(drawLInfo.pInfo[i].bShape);

			setPenStyle(drawLInfo.pInfo[i].pWidth, drawLInfo.pInfo[i].lParam, drawLInfo.pInfo[i].pColor);

			x = LOWORD(drawLInfo.pInfo[i].lParam);
			y = HIWORD(drawLInfo.pInfo[i].lParam);

			switch (drawLInfo.pInfo[i].state)
			{
			case WM_LBUTTONDOWN:
				MoveToEx(hdc, x, y, NULL);
				LineTo(hdc, x, y + 1);
				break;

			case WM_MOUSEMOVE:
				LineTo(hdc, x, y);
				break;

			case WM_LBUTTONUP:
				LineTo(hdc, x, y);
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
	case PENCIL: // 연필 (색깔만 회색으로)
		nPen = CreatePen(PS_SOLID, size, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;
	case SPRAY: // 스프레이 (점을 흩뿌림)
		for (int i = 0; i < 100; ++i)
		{
			int offsetX = (rand() % (size * 8)) - (size * 4);
			int offsetY = (rand() % (size * 8)) - (size * 4);
			if (sqrt(offsetX * offsetX + offsetY * offsetY) <= size * 4)
			{
				SetPixel(hdc, x + offsetX, y + offsetY, col);
			}
		}
		ReleaseDC(hWnd, hdc);
		break;
	case MARKER:
		for (int i = 0; i < 100; ++i)
		{
			int offsetX = (rand() % (size * 2)) - size;
			int offsetY = (rand() % (size * 2)) - size;
			SetPixel(hdc, x + offsetX, y + offsetY, col);
		}
		ReleaseDC(hWnd, hdc);
		break;
	case RECTANGLE:

		hPen = CreateSolidBrush(RGB(GetRValue(col), GetGValue(col), GetBValue(col)));
		SelectObject(hdc, hPen);
		Rectangle(hdc, x - size, y - size, x + size, y + size);
		ReleaseDC(hWnd, hdc);
		break;

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