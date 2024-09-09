#pragma once

#include <windows.h>
#include <commdlg.h> /// ���� ������ ����ϱ����� API 

class ColorPalette {

private:
	COLORREF acrCustClr[16];									/// ����� ���� ���� ���� ����
	CHOOSECOLOR cc;											/// ���� ���� â�� ����, ȣ���ϴ� ����ü
	HBRUSH hBrush = NULL;		/// ������ �����ϴ� �귯�� => �ʱⰪ NULL
	COLORREF colorArr[3] = { RGB(0,0,0),RGB(0,0,0), RGB(0,0,0) };

public:
	void colorSelect(HWND, int);
	void paint(PAINTSTRUCT, HDC);
	void destroy();

	COLORREF getColor(int);

};