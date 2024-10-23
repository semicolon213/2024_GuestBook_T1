#pragma once
#include "framework.h"
#include <vector>
#include "Resource.h"
class MakeButton 
{
private:
	int icon;

public:
	RECT rectButton;
	bool toggleState;

	/// 기본 생성자
	MakeButton();

	/// 좌표 지정 생성자
	MakeButton(int left, int top, int right, int bottom);

	void drawRectButton(HDC tHdc, int icon);

	void drawEllipseButton(HDC tHdc, COLORREF test);

	void setCoordinate(int left, int top, int right, int bottom);

	/// <summary>
	/// 클릭시 버튼에 이펙트 설정 메서드
	/// </summary>
	/// <param name="icon"> 설정할 이펙트 아이콘 </param>
	/// <param name="clickIcon"> 내가 클릭한 아이콘 </param>
	/// <param name="tHdc"> HDC </param>
	void clickEffectPen(int icon,int clickIcon, HDC tHdc);

	void clickEffectPen(int icon, HDC tHdc);
	
	void doubleImgButton(HDC tHdc, int icon1, int icon2);
	
protected:
	void buttonLoadImage(int icon, HDC tHdc);
};
