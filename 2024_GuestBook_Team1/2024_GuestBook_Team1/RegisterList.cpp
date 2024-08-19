#include "RegisterList.h"

void RegisterList::timer(HWND hWnd) 
{
	listPos -= 2;

	if (listPos + listWidth < 0)
	{
		listPos = sWidth;
	}

	InvalidateRect(hWnd, NULL, true);
}

void RegisterList::paint(HDC hdc)
{
	GetTextExtentPoint32(hdc, list, lstrlen(list), &size);
	listWidth = size.cx;

	TextOut(hdc, listPos, 50, list, lstrlen(list));
}

void RegisterList::getsWidth(LPARAM lParam)
{
	sWidth = LOWORD(lParam);
}