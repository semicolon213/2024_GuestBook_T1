#pragma once

#include <Windows.h>

#define ID_RIGHT_PANEL 102

// �г� �Լ�
void InitializePanels(HWND hWnd);
void ResizePanels(HWND hWnd, LPARAM lParam);

// ���� ����
extern HWND hLeftPanel, hRightPanel;
