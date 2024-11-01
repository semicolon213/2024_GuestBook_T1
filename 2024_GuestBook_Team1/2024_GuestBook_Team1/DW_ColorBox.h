#pragma once
#include <Windows.h>
#include "DrowWindow.h"
#include <Windowsx.h>
#include <cmath>
#include <string>
class DW_ColorBox
{
public:
    static int colorSelect;

    static COLORREF colorP[3];

    static COLORREF getColorNum(int);

    static void setColorNum(int num, COLORREF color);

    void DrawColorWheel(HDC hdc, int centerX, int centerY, int radius);
    void DrawColorBar(HDC hdc, RECT rect);
    void DrawSelectedColor(HDC hdc, RECT rect, COLORREF color);
    void DrawSlider(HDC hdc, RECT rect, int value, int max);
    COLORREF HSVtoRGB(double h, double s, double v);
    void RGBtoHSV(COLORREF rgb, double& h, double& s, double& v);

};

