/**
@file RegisterList.h
@brief �湮�� ��� ������ Ŭ���� ��� ���� ����
*/

#pragma once
#include <Windows.h>


class RegisterList
{
private :
	int listPos = 0;
	int sWidth = 0;
	int listWidth = 0;

	SIZE size;
	LPCWSTR list = L"�湮�� �ֽ� �е� : Test1, Test2, Test3, Test4";

public :
	void timer(HWND);
	void paint(HDC);
	void getsWidth(LPARAM);

};

