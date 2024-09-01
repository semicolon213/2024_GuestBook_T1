#pragma once

#include <Windows.h>
#include <string>

#define ID_RIGHT_PANEL 102
#define ID_FILE_LIST 101
#define ID_BUTTON_SAVE 200


/*패널 함수*/ 
void InitializePanels(HWND hWnd);
void ResizePanels(HWND hWnd, LPARAM lParam);

/*버튼 함수*/ 
void AddFileToList(const std::wstring& fileName);
void Button(HWND hWnd);
void UpdateFileListUI();

/* 전역 변수 */
extern HWND hLeftPanel, hRightPanel;
extern HWND hFileListBox;
extern HINSTANCE hInst;



