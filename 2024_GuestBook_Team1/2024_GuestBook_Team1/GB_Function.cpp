#include "GB_Function.h"

using namespace std;


void GB_Function::record(LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)
{
	drawPInfo.lParam = lParam;
	drawPInfo.pTime = pTime;
	drawPInfo.state = state;
	drawPInfo.pWidth = size;
	drawPInfo.pColor = col;
	drawLInfo.pInfo.push_back(drawPInfo);
}

void GB_Function::draw(HWND hWnd, LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)
{
	hdc = GetDC(hWnd);
	if (isLeftClick)
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
	
		nPen = CreatePen(PS_SOLID, size, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
	
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, px, py);

		px = x;
		py = y;

		record(lParam, pTime, state, size, col);
	
	}

}

void GB_Function::mouseUD(LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)
{
	isLeftClick = !isLeftClick;
	
	x = LOWORD(lParam);
	y = HIWORD(lParam);

	if(isLeftClick) 
	{

		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x, y);

		px = x;
		py = y;

	}

	record(lParam, pTime, state, size, col);
}

//DWORD GB_Function::replay(LPVOID points)
//{
//	
//}