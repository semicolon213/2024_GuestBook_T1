#pragma once
#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include "Pen_Str.h"
#include "2024_GuestBook_Team1.h"


class Stamp {
private:
    int stampSize;                 // ������ ũ��
    int stampIcon;                 // ������ ������ ID
    int stampX, stampY;            // �������� x, y ��ǥ
    RECT stamptext;

public:
    Stamp(int size, int icon);

    void changeModeToStamp(bool* stampActive, HWND g_Hwnd, int* stampIcon, int wParam);
    void changeModeToPen(HWND g_Hwnd, bool* stampActive);

    void handleStamp(HWND hWnd, UINT message, LPARAM lParam, std::vector<PEN_INFO>* penMemory);

private:
    void drawIcon(HDC hdc, int x, int y, HICON hIcon); // ������ �������� �׸��� �޼���
};
