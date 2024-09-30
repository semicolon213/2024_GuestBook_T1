#include "ColorPalette.h"

<<<<<<< HEAD
/// 사용자가 색상 선택 대화 상자를 통해 색상을 선택하고, 이를 색상 배열에 저장하는 함수
=======
COLORREF ColorPalette::colorArr[] = { RGB(0,0,0),RGB(255,0,0), RGB(0,255,0) };

>>>>>>> origin/develop
void ColorPalette::colorSelect(HWND hWnd, int penNum)
{
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc); /// cc 구조체의 크기 설정
    cc.hwndOwner = hWnd; 
    cc.lpCustColors = (LPDWORD)acrCustClr; 
    cc.rgbResult = RGB(0, 0, 0); 
    cc.Flags = CC_FULLOPEN | CC_RGBINIT; 

<<<<<<< HEAD
    if (ChooseColor(&cc)) { /// 색상 선택 대화 상자 호출, 사용자가 색상을 선택하면 TRUE 반환
        if (hBrush) { 
            DeleteObject(hBrush); 
        }
        hBrush = CreateSolidBrush(cc.rgbResult); 
        colorArr[penNum] = cc.rgbResult; 
        InvalidateRect(hWnd, NULL, TRUE); 
    }
}

/// 선택된 색상을 사용하여 윈도우를 페인트하는 함수
void ColorPalette::paint(PAINTSTRUCT ps, HDC hdc)
{
    if (hBrush)
    {
        FillRect(hdc, &ps.rcPaint, hBrush); 
    }
}

/// 브러시 객체를 삭제하여 리소스를 해제
void ColorPalette::destroy()
{
    if (hBrush)
    {
        DeleteObject(hBrush); 
    }
}

COLORREF ColorPalette::getColor(int penNum) {
    return colorArr[penNum]; /// 펜 번호에 맞는 색상을 반환
}
=======
	if (ChooseColor(&cc))			/// 색상 선택 대화 상자 표시
	{
		colorArr[penNum] = cc.rgbResult;
	}
}



COLORREF ColorPalette::getColor(int penNum) {
	return ColorPalette::colorArr[penNum];
}
>>>>>>> origin/develop
