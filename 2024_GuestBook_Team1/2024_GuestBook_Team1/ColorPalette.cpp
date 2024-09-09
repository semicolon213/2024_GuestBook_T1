#include "ColorPalette.h" /// colorpalette.h 헤더 파일을 포함한다

void ColorPalette::colorSelect(HWND hWnd, int penNum)
{
	ZeroMemory(&cc, sizeof(cc)); /// 색상 선택 창 설정, 호출을 기본값으로 설정
	cc.lStructSize = sizeof(cc); /// 구조체의 크기 설정
	cc.hwndOwner = hWnd; /// 소유자 핸들 설정
	cc.lpCustColors = (LPDWORD)acrCustClr; /// lpCustColors : 사용자 정의 색상 배열의 주소
	cc.rgbResult = RGB(0, 0, 0); /// 색상 창이 처음 열릴 떄 보여지는 색상 (초기 색상 : 검은색)
	cc.Flags = CC_FULLOPEN | CC_RGBINIT; /// 대화 상자 플래그 설정

	if (ChooseColor(&cc)) { /// 색상 선택 대화 상자 표시
		if (hBrush) {
			DeleteObject(hBrush); /// 기존 브러쉬 삭제
		}
		hBrush = CreateSolidBrush(cc.rgbResult); /// 새로운 브러쉬 생성
		colorArr[penNum] = cc.rgbResult;
		InvalidateRect(hWnd, NULL, TRUE);
	}
}

void ColorPalette::paint(PAINTSTRUCT ps, HDC hdc)
{
	if (hBrush)
	{
		FillRect(hdc, &ps.rcPaint, hBrush);
	}

}

void ColorPalette::destroy()
{
	if (hBrush)
	{
		DeleteObject(hBrush); /// 브러쉬 삭제
	}
}

COLORREF ColorPalette::getColor(int penNum) {
	return colorArr[penNum];
}