#pragma once
#pragma comment(lib, "Gdiplus.lib")

#include <windows.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <cmath>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

class ColorPicker {
private:
    HWND hWnd;
    HWND hSlider;              // �����̴� �ڵ�
    RECT colorRect[16];        // ���� ���� ����
    RECT previewRect;          // �̸����� ����
    RECT thicknessRect;        // ���� ���� 
    int palette_x_;
    int palette_y_;
    int palette_width_;
    int palette_height_;

    COLORREF selectedColor;    // ���õ� ����
    int thickness;             // ���õ� ����

    HBRUSH colorBrush[16];     // ���� ������ ���� �귯�� �迭
    void PaletteControl(POINT mouse_position);
    double h_; // ����(Hue)
    double s_ = 1; // ä��(Saturation)
    double v_; // ��(Value)
    double pen_size_; // �� ũ��

    bool is_color_picker_open_;
    Gdiplus::Color current_color_;

public:
    ColorPicker(HWND hWnd);
    ~ColorPicker();
    void showPicker(HWND parentWnd);
    static LRESULT CALLBACK ColorPickerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void drawColors(HDC hdc);
    void drawPreview(HDC hdc);
    void drawThickness(HDC hdc);
    void handleColorSelection(HWND hWnd, int x, int y);
    void Draw(HDC hdc);

    Gdiplus::Color HSVToRGB(double h, double s, double v);
};