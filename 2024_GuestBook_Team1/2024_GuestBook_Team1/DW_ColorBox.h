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
    static int colorSelect;// ���� ���õ� ���� ��ư �ε��� 

    static COLORREF colorP[3];// �� ���� ��ư�� ����� ���� 
    static int thicknessP[3]; // �� ���� ��ư�� ����� ����

    static COLORREF getColorNum(int);

    static void setColorNum(int num, COLORREF color);

    static int getThicknessNum(int colorSelect);  // ���� ���� �������� �޼���
    static void setThicknessNum(int num, int thickness); // ���� ���� �����ϴ� �޼���

    void DrawColorWheel(HDC hdc, int centerX, int centerY, int radius);
    void DrawColorBar(HDC hdc, RECT rect);
    void DrawSelectedColor(HDC hdc, RECT rect, COLORREF color);
    void DrawSlider(HDC hdc, RECT rect, int value, int max);
    void DrawThicknessSlider(HDC hdc, RECT rect, int thicknessValue, int maxThickness); // ���� �����̴�
    COLORREF HSVtoRGB(double h, double s, double v);
    void RGBtoHSV(COLORREF rgb, double& h, double& s, double& v);
    // �����̴��� �ﰢ�� ��Ŀ�� �����ϱ� ���� �޼���
    void DrawSliderMarker(HDC hdc, RECT rect, int value, int max);
    // �����̴� ��Ŀ�� ��ġ�� ��� �����ϴ� �Լ�
    void UpdateSliderMarker(HWND hWnd, RECT rect, int value, int max);
    void DrawColorButton(HDC hdc, RECT rect, COLORREF color); // ���� ��ư�� �׸��� �޼���
    void UpdateColorButton(HWND hWnd); // ������ ����� �� ��ư�� ������Ʈ�ϴ� �޼���


};

