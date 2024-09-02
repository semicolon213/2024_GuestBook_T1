#pragma once

#include <Windows.h>
#include <string>

#define ID_RIGHT_PANEL 102
#define ID_FILE_LIST 101
#define ID_BUTTON_SAVE 200


/*�г� �Լ�*/ 
void InitializePanels(HWND hWnd);
void ResizePanels(HWND hWnd, LPARAM lParam);

/*��ư �Լ�*/ 
void AddFileToList(const std::wstring& fileName);
void Button(HWND hWnd);
void UpdateFileListUI();

/* ���� ���� */
extern HWND hLeftPanel, hRightPanel;
extern HWND hFileListBox;
extern HINSTANCE hInst;



