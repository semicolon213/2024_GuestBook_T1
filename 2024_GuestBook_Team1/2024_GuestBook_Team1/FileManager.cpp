#include "FileManager.h"


HWND hRightPanel;

// 패널을 초기화
void InitializePanels(HWND hWnd)
{
    static const int fixedWidth = 300; // 오른쪽 패널의 고정된 너비를 정의 

    // 오른쪽 패널을 생성
    hRightPanel = CreateWindowW(L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, fixedWidth, 600, hWnd, (HMENU)ID_RIGHT_PANEL, GetModuleHandle(NULL), NULL);


}

// 패널 크기 
void ResizePanels(HWND hWnd, LPARAM lParam)
{
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    static const int fixedWidth = 300;

    // 패널 크기 조정
    MoveWindow(hRightPanel, width - fixedWidth, 0, fixedWidth, height, TRUE);

}
