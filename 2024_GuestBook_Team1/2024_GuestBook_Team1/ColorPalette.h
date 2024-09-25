#pragma once

#include <windows.h>
#include <commdlg.h> /// 색상 선택을 사용하기위한 API 

class ColorPalette {

private:
	COLORREF acrCustClr[16];	/// 사용자 정의 색상 저장 공간
	CHOOSECOLOR cc;				/// 색상 선택 창을 설정, 호출하는 구조체
	HBRUSH hBrush = NULL;		/// 색상을 저장하는 브러쉬 초기값 NULL
	COLORREF colorArr[3] = { RGB(0,0,0),RGB(0,0,0), RGB(0,0,0) }; /// 펜 번호에 따른 색상 저장 배열 초기값 검정색

public:
	void colorSelect(HWND, int); /// 색상 선택 대화 상자를 표시, 선택된 색상 저장
	void paint(PAINTSTRUCT, HDC); /// 선택된 색상을 사용해 창을 페인트
	void destroy(); /// 생성된 브러시 객체를 삭제

	COLORREF getColor(int); /// 펜 번호에 따른 색상 변환

};