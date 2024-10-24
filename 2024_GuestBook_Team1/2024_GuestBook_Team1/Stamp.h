#pragma once
#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include "Pen_Str.h"
#include "2024_GuestBook_Team1.h"


class Stamp {
private:
    int stampSize;                 // 스탬프 크기
    int stampIcon;                 // 스탬프 아이콘 ID
    int stampX, stampY;            // 스탬프의 x, y 좌표
    RECT stamptext;

public:
    Stamp(int size, int icon);

    void changeModeToStamp(bool* stampActive, HWND g_Hwnd, int* stampIcon, int wParam);
    void changeModeToPen(HWND g_Hwnd, bool* stampActive);

    void handleStamp(HWND hWnd, UINT message, LPARAM lParam, std::vector<PEN_INFO>* penMemory);

private:
    void drawIcon(HDC hdc, int x, int y, HICON hIcon); // 스탬프 아이콘을 그리는 메서드
};
