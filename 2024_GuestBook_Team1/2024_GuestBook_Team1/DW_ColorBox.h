#pragma once
#include <Windows.h>
#include "DrowWindow.h"
#include <Windowsx.h>
#include <cmath>
#include <string>
#include "DW_ToolMenu.h"
class DW_ColorBox
{
public:
    static int colorSelect;// 현재 선택된 색상 버튼 인덱스 

    static COLORREF colorP[3];// 각 색상 버튼에 저장된 색상 
    static int thicknessP[3]; // 각 색상 버튼에 저장된 굵기

    static COLORREF getColorNum(int);

    static void setColorNum(int num, COLORREF color);

    static int getThicknessNum(int colorSelect);  // 굵기 값을 가져오는 메서드
    static void setThicknessNum(int num, int thickness); // 굵기 값을 설정하는 메서드

    void DrawColorWheel(HDC hdc, int centerX, int centerY, int radius);
    void DrawColorBar(HDC hdc, RECT rect);
    void DrawSelectedColor(HDC hdc, RECT rect, COLORREF color);
    void DrawSlider(HDC hdc, RECT rect, int value, int max);
    void DrawThicknessSlider(HDC hdc, RECT rect, int thicknessValue, int maxThickness); // 굵기 슬라이더
    COLORREF HSVtoRGB(double h, double s, double v);
    void RGBtoHSV(COLORREF rgb, double& h, double& s, double& v);
    // 슬라이더의 삼각형 마커만 갱신하기 위한 메서드
    void DrawSliderMarker(HDC hdc, RECT rect, int value, int max);
    // 슬라이더 마커의 위치를 즉시 갱신하는 함수
    void UpdateSliderMarker(HWND hWnd, RECT rect, int value, int max);
    void DrawColorButton(HDC hdc, RECT rect, COLORREF color); // 색상 버튼을 그리는 메서드
    void UpdateColorButton(HWND hWnd); // 색상이 변경될 때 버튼을 업데이트하는 메서드


};

