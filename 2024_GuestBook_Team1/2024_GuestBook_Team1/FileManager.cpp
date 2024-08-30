#include "FileManager.h"


HWND hRightPanel;

// �г��� �ʱ�ȭ
void InitializePanels(HWND hWnd)
{
    static const int fixedWidth = 300; // ������ �г��� ������ �ʺ� ���� 

    // ������ �г��� ����
    hRightPanel = CreateWindowW(L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, fixedWidth, 600, hWnd, (HMENU)ID_RIGHT_PANEL, GetModuleHandle(NULL), NULL);


}

// �г� ũ�� 
void ResizePanels(HWND hWnd, LPARAM lParam)
{
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    static const int fixedWidth = 300;

    // �г� ũ�� ����
    MoveWindow(hRightPanel, width - fixedWidth, 0, fixedWidth, height, TRUE);

}
