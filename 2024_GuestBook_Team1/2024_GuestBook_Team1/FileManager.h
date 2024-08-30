#pragma once

#include <Windows.h>

#define ID_RIGHT_PANEL 102

// 패널 함수
void InitializePanels(HWND hWnd);
void ResizePanels(HWND hWnd, LPARAM lParam);

// 전역 변수
extern HWND hLeftPanel, hRightPanel;
