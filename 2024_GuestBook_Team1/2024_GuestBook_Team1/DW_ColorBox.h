#pragma once
#include "Window.h"
#include "DrowWindow.h"
class DW_ColorBox
{
public:
    static int colorSelect;

    static COLORREF colorP[3];

    static COLORREF getColorNum();

    static void setColorNum(int num, COLORREF color);
};

