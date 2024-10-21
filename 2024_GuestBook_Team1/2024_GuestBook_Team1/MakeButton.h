#pragma once
#include "framework.h"




class MakeButton 
{
public:
	RECT rectButton;

private:
	int icon;

public:

	MakeButton(int left, int top, int right, int bottom);

	void drawRectButton(HDC tHdc, int icon);
	void drawEllipseButton(HDC tHdc);

protected:
	void buttonLoadImage(int icon, HDC tHdc);

};
