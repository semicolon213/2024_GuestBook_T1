/**
@author �����
@date 2024.09.01
    ���ϸŴ��� ��� ������ ���� �ӽ� �����ư ���� 
    ���ϸ���Ʈ ���� (��ǥ�� ��ġ �� ui�� ��� ���� �Ϸ� �� �����۾� ����)
@todo �����ư ������ ���ϸ���Ʈ�� ����
      ��ǥ ���ġ �� UI ������Ʈ 
      ���̾�α׿� �����ϱ� �ҷ����� ���� 
**/



#include "FileManager.h"
#include <vector>
#include <string>
#include "window.h"

std::vector<std::wstring> fileList;

HWND hFileListBox;
HWND hRightPanel;
HWND File_Manager_Button;


/*�г��� �ʱ�ȭ*/ 
void InitializePanels(HWND hWnd)
{
    static const int fixedWidth = 300; // ������ �г��� ������ �ʺ� ���� 

    /* ������ �г��� ���� */
    hRightPanel = CreateWindowW(L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, fixedWidth, 600, hWnd, (HMENU)ID_RIGHT_PANEL, GetModuleHandle(NULL), NULL);

    /* ���� ����Ʈ �ڽ� ���� */
    hFileListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, fixedWidth - 20, 750, hRightPanel, (HMENU)ID_FILE_LIST, GetModuleHandle(NULL), NULL);

 
}

/* �г� ũ��*/
void ResizePanels(HWND hWnd, LPARAM lParam)
{
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    static const int fixedWidth = 300;

    /* �г� ũ�� ���� */
    MoveWindow(hRightPanel, width - fixedWidth, 0, fixedWidth, height, TRUE);
    
}void AddFileToList(const std::wstring& fileName)
{
    fileList.push_back(fileName);
    UpdateFileListUI();
}

void UpdateFileListUI()
{
    /*����Ʈ �ڽ��� ���*/
    SendMessage(hFileListBox, LB_RESETCONTENT, 0, 0);

    /* ���� ����Ʈ�� ����Ʈ �ڽ��� �߰�*/
    for (const auto& file : fileList)
    {
        SendMessage(hFileListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
    }
}
void Button(HWND hWnd) /*���ϸŴ��� �������� ���� �ӽ� ��ư */
{
    File_Manager_Button = CreateWindowW(L"Button", L"����", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150,150,150,150, hWnd, (HMENU)ID_BUTTON_SAVE, GetModuleHandle(NULL), NULL);

    if (!File_Manager_Button) /*��ư ������ �ڲ� ������ ���� Ȯ�ο�.*/
    {
        MessageBox(hWnd, L"��ư ���� ����", L"����", MB_OK | MB_ICONERROR);
    }
}

/*void SaveFile(const std::wstring& fileName) 
{
    // ���� ���� ���� �ֱ�
    

    AddFileToList(fileName); 
} ���� ��� ���� �� */





