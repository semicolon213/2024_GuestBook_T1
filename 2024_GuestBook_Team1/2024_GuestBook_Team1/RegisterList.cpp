/**
@file RegisterList.cpp
@brief �湮�� ��� ������ Ŭ���� �ܺ� ���� ����
*/

#include "RegisterList.h"

/**
@fn  RegisterList::timer(HWND hWnd)
@brief WM_TIMER�� �Էµ� ������ �湮�� ����� ��ġ�� �������� �ű�� �޼ҵ�
@param hWnd ������ ��鷯�� ������
*/

void RegisterList::timer(HWND hWnd)
{
	listPos -= 2;

	if (listPos + listWidth < 0)
	{
		listPos = sWidth;
	}

	InvalidateRect(hWnd, NULL, true);
}

/**
@fn  RegisterList::paint(HDC hdc)
@brief 
@param 
*/

void RegisterList::paint(HDC hdc)
{
	GetTextExtentPoint32(hdc, list, lstrlen(list), &size);
	listWidth = size.cx;

	TextOut(hdc, listPos, 50, list, lstrlen(list));
}

/**
@fn  RegisterList::getsWidth(LPARAM lParam)
@brief 
@param 
*/

void RegisterList::getsWidth(LPARAM lParam)
{
	sWidth = LOWORD(lParam);
}