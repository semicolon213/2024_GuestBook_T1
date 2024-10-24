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
    HWND hSlider;              // 슬라이더 핸들
    RECT colorRect[16];        // 색상 선택 영역
    RECT previewRect;          // 미리보기 영역
    RECT thicknessRect;        // 굵기 선택 
    int palette_x_;
    int palette_y_;
    int palette_width_;
    int palette_height_;

    COLORREF selectedColor;    // 선택된 색상
    int thickness;             // 선택된 굵기

    HBRUSH colorBrush[16];     // 색상 선택을 위한 브러시 배열
    void PaletteControl(POINT mouse_position);
    double h_; // 색상(Hue)
    double s_ = 1; // 채도(Saturation)
    double v_; // 명도(Value)
    double pen_size_; // 팬 크기

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