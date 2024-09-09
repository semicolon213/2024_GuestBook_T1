#pragma once

#include <windows.h>
#include <commdlg.h> /// 색상 선택을 사용하기위한 API 

class ColorPalette {

private:
	COLORREF acrCustClr[16];									/// 사용자 정의 색상 저장 공간
	CHOOSECOLOR cc;											/// 색상 선택 창을 설정, 호출하는 구조체
	HBRUSH hBrush = NULL;		/// 색상을 저장하는 브러쉬 => 초기값 NULL
	COLORREF colorArr[3] = { RGB(0,0,0),RGB(0,0,0), RGB(0,0,0) };

public:
	void colorSelect(HWND, int);
	void paint(PAINTSTRUCT, HDC);
	void destroy();

	COLORREF getColor(int);

};