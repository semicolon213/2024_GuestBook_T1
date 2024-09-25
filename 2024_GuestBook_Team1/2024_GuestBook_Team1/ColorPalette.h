#pragma once

#include <windows.h>
#include <commdlg.h> /// ���� ������ ����ϱ����� API 
class ColorPalette {

private:
	COLORREF acrCustClr[16];									/// ����� ���� ���� ���� ����
	CHOOSECOLOR cc;											/// ���� ���� â�� ����, ȣ���ϴ� ����ü
	HBRUSH hBrush = NULL;		/// ������ �����ϴ� �귯�� => �ʱⰪ NULL
	

public:
	void colorSelect(HWND, int);

	COLORREF getColor(int);

	static COLORREF colorArr[3];

};