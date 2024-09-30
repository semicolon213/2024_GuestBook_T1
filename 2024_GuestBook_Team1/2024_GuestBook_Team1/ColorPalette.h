#pragma once

#include <windows.h>
<<<<<<< HEAD
#include <commdlg.h> 

/// 색상 선택 및 페인팅 기능을 제공하는 클래스
class ColorPalette {

private:
    COLORREF acrCustClr[16];  /// 사용자 정의 색상을 저장하는 배열 (16개의 사용자 색상 저장 가능)
    CHOOSECOLOR cc;           /// 색상 선택 대화 상자를 설정하고 호출
    HBRUSH hBrush = NULL;     /// 현재 선택된 색상으로 생성된 브러시, 초기값은 NULL
    COLORREF colorArr[3] = { RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) }; /// 펜 번호에 따른 색상을 저장(3개의 색상, 초기값은 검정색)

public:
    /// 색상 선택 대화 상자를 표시하고 선택된 색상을 저장하는 함수
    void colorSelect(HWND, int);
=======
#include <commdlg.h> /// 색상 선택을 사용하기위한 API 
class ColorPalette {

private:
	COLORREF acrCustClr[16];									/// 사용자 정의 색상 저장 공간
	CHOOSECOLOR cc;											/// 색상 선택 창을 설정, 호출하는 구조체
	HBRUSH hBrush = NULL;		/// 색상을 저장하는 브러쉬 => 초기값 NULL
	

public:
	void colorSelect(HWND, int);
>>>>>>> origin/develop

    /// 선택된 색상을 사용해 창을 페인트하는 함수
    void paint(PAINTSTRUCT, HDC);

<<<<<<< HEAD
    /// 생성된 브러시 객체를 삭제하는 함수
    void destroy();

    /// 펜 번호에 따른 색상을 반환하는 함수
    COLORREF getColor(int);
};
=======
	static COLORREF colorArr[3];

};
>>>>>>> origin/develop
