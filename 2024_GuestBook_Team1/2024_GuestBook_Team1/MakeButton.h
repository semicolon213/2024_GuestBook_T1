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

	MakeButton();
	MakeButton(int left, int top, int right, int bottom);

	void drawRectButton(HDC tHdc, int icon);
	void drawEllipseButton(HDC tHdc);

	void setCoordinate(int left, int top, int right, int bottom);

	void clickEffectPen(int icon,int clickIcon, HDC tHdc);

	void clickEffectPen(int icon, HDC tHdc);
	
	void doubleImgButton(HDC tHdc, int icon1, int icon2);
	
protected:
	void buttonLoadImage(int icon, HDC tHdc);
};
