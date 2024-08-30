/**
@file RegisterList.cpp
@brief 방문자 명단 전광판 클래스 외부 정의 파일
*/

#include "RegisterList.h"

/**
@fn  RegisterList::timer(HWND hWnd)
@brief WM_TIMER가 입력될 때마다 방문자 명단의 위치를 왼쪽으로 옮기는 메소드
@param hWnd 윈도우 헨들러를 전달함
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