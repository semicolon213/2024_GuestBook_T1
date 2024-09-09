/**
@author �����
@date 2024.09.08
    ���ϸ���Ʈ ���� (��ǥ�� ��ġ �� ui�� ��� ���� �Ϸ� �� �����۾� ����)
    �����ư ������ ���ϸ���Ʈ�� ����
    ���� ��ο� ����
@todo �ҷ����� �����
      ��ǥ ���ġ �� UI ������Ʈ 
      ���̾�α׿� �����ϱ� �ҷ����� ���� 
**/



#include "FileManager.h"

std::vector<std::wstring> fileList;

FileManager::FileManager() : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), hWnd(nullptr) {
    
}

// HWND�� �޴� ������ ����
FileManager::FileManager(HWND hWnd) : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), hWnd(hWnd) {
    
}


/*�г��� �ʱ�ȭ*/ 
void FileManager::InitializePanels(HWND hWnd)
{
    static const int fixedWidth = 300; // ������ �г��� ������ �ʺ� ���� 

    /* ������ �г��� ���� */
    getInstance().hRightPanel = CreateWindowW(L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, fixedWidth, 600, hWnd, (HMENU)ID_RIGHT_PANEL, GetModuleHandle(NULL), NULL);

    /* ���� ����Ʈ �ڽ� ���� */
    getInstance().hFileListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, fixedWidth - 20, 750, getInstance().hRightPanel, (HMENU)ID_FILE_LIST, GetModuleHandle(NULL), NULL);

 
}

/* �г� ũ��*/
void FileManager::ResizePanels(HWND hWnd, LPARAM lParam)
{
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    static const int fixedWidth = 300;

    /* �г� ũ�� ���� */
    MoveWindow(getInstance().hRightPanel, width - fixedWidth, 0, fixedWidth, height, TRUE);
    
}void FileManager::AddFileToList(const std::wstring& fileName)
{
    fileList.push_back(fileName);
    UpdateFileListUI();
}

void FileManager::UpdateFileListUI()
{
    /*����Ʈ �ڽ��� ���*/
    SendMessage(getInstance().hFileListBox, LB_RESETCONTENT, 0, 0);

    /* ���� ����Ʈ�� ����Ʈ �ڽ��� �߰�*/
    for (const auto& file : fileList)
    {
        SendMessage(getInstance().hFileListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
    }
}





void FileManager::SaveFile() {
    WCHAR file_name[256] = L"";
    OPENFILENAME OFN;
    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd; 
    OFN.lpstrFilter = L"Guest Book(*.gb)\0*.gb";
    OFN.lpstrDefExt = L"gb";
    OFN.lpstrFile = file_name;
    OFN.nMaxFile = 256;
    OFN.lpstrInitialDir = L"C:\\Users\\Public\\Documents";
    OFN.Flags = OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&OFN)) {
        
        std::wofstream outFile(file_name);  /// ���ϻ���

        if (outFile.is_open()) {
            
            
            outFile.close();  // ���� �ݱ�

            AddFileToList(file_name);  /// ������ ���ϸ��� ����Ʈ�� �߰�
        }
        else {
           
            MessageBox(hWnd, L"���� ���忡 ����!", L"����", MB_OK | MB_ICONERROR);
        }
    }
    
}



