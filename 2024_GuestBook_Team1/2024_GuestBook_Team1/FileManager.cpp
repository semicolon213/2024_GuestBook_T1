/**
 * @file FileManager.cpp
 * @brief ���� �Ŵ��� ���� Ŭ���� �ܺ� ���� ����. ���� ����, �ҷ�����, ����Ʈ ���� ��� ����.
 * @author �����
 * @date 2024.09.08
 *
 * 
 * @todo UI������Ʈ�� �°� ��ǥ �� ��� ���ġ, ���� �� �ҷ����⿡�� ������ ���� ����� �ȵǴ� ���� �ذ� 
 */




#include "FileManager.h"

std::vector<std::wstring> fileList;

/**
 * @fn FileManager::FileManager()
 * @brief FileManager �⺻ ������.
 */
FileManager::FileManager() : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), hWnd(nullptr) {}

/**
 * @fn FileManager::FileManager(HWND hWnd)
 * @brief HWND�� ���ڷ� �޴� ������.
 * @param hWnd ������ �ڵ�.
 */
FileManager::FileManager(HWND hWnd) : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), hWnd(hWnd) {}

/**
 * @fn FileManager::InitializePanels(HWND hWnd)
 * @brief �г� �ʱ�ȭ. ������ �гΰ� ���� ����Ʈ �ڽ��� ����.
 * @param hWnd ������ �ڵ�.
 */
void FileManager::InitializePanels(HWND hWnd)
{
    static const int fixedWidth = 300; // ������ �г��� ������ �ʺ�

    // ������ �г� ����
    getInstance().hRightPanel = CreateWindowW(L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, fixedWidth, 600, hWnd, (HMENU)ID_RIGHT_PANEL, GetModuleHandle(NULL), NULL);

    // ���� ����Ʈ �ڽ� ����
    getInstance().hFileListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, fixedWidth - 20, 750, getInstance().hRightPanel, (HMENU)ID_FILE_LIST, GetModuleHandle(NULL), NULL);
}

/**
 * @fn FileManager::ResizePanels(HWND hWnd, LPARAM lParam)
 * @brief ������ ũ�⿡ ���� �г� ũ�⸦ ����.
 * @param hWnd ������ �ڵ�.
 * @param lParam ũ�� �Ķ����.
 */
void FileManager::ResizePanels(HWND hWnd, LPARAM lParam)
{
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    static const int fixedWidth = 300;

    /// �г� ũ�� ����
    MoveWindow(getInstance().hRightPanel, width - fixedWidth, 0, fixedWidth, height, TRUE);
}

/**
 * @fn FileManager::AddFileToList(const std::wstring& fileName)
 * @brief ���� ����Ʈ�� ���� �߰� �� UI ������Ʈ.
 * @param fileName �߰��� ���� �̸�.
 */
void FileManager::AddFileToList(const std::wstring& fileName)
{
    fileList.push_back(fileName);
    UpdateFileListUI();
}

/**
 * @fn FileManager::UpdateFileListUI()
 * @brief ���� ����Ʈ UI ������Ʈ. ����Ʈ �ڽ��� ������.
 */
void FileManager::UpdateFileListUI()
{
    /// ����Ʈ �ڽ� ����
    SendMessage(getInstance().hFileListBox, LB_RESETCONTENT, 0, 0);

    /// ���� ����Ʈ �߰�
    for (const auto& file : fileList)
    {
        SendMessage(getInstance().hFileListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
    }
}

/**
 * @fn FileManager::FileManager_Open()
 * @brief �г� ����/���� ���¸� �����.
 */
void FileManager::FileManager_Open()
{
    is_open_ = !is_open_;  /// ����/���� ���� ���

    if (is_open_) {
        ShowWindow(getInstance().hRightPanel, SW_SHOW);  /// �г� ǥ��
    }
    else {
        ShowWindow(getInstance().hRightPanel, SW_HIDE);  /// �г� �����
    }

    InvalidateRect(hWnd, NULL, FALSE);  /// â ����
}

/**
 * @fn FileManager::SaveFile(HWND hWnd, std::vector<PINFO>* penMemory)
 * @brief ���� ���� ���. ���� ��ư Ŭ�� �� ȣ���.
 * @param hWnd ������ �ڵ�.
 * @param penMemory ������ �����͸� �����ϴ� ������.
 */
void FileManager::SaveFile(HWND hWnd, std::vector<PINFO>* penMemory) {
    WCHAR file_name[256] = L"";
    OPENFILENAME OFN;
    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFilter = L"Guest Book(*.gb)\0*.gb";
    OFN.lpstrDefExt = L"gb";
    OFN.lpstrFile = file_name;
    OFN.nMaxFile = 256;
    OFN.lpstrInitialDir = L"C:\\Users\\siuugil\\Test\\2024_GuestBook_Team1\\savefile";
    OFN.Flags = OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&OFN)) {
        std::fstream outFile(file_name, std::ios::out);  // ���� ����

        if (!outFile.is_open()) {
            MessageBox(hWnd, L"������ �� �� �����ϴ�.", L"����", MB_OK | MB_ICONERROR);
            return;
        }

        // ������ ���Ͽ� ����
        for (const auto& data : *penMemory) {
            outFile << data.lParam << L' '
                << data.pWidth << L' '
                << data.pColor << L' '
                << data.pTime << L' '
                << data.state << L' '
                << data.bShape << std::endl;
        }

        outFile.close();
        AddFileToList(file_name);  // ���� ����Ʈ�� �߰�
    }
    else {
        MessageBox(hWnd, L"���� ������ ��ҵǾ����ϴ�.", L"����", MB_OK | MB_ICONWARNING);
    }
}

/**
 * @fn FileManager::LoadFile(HWND hWnd, std::vector<PINFO>* penMemory)
 * @brief ���� �ҷ����� ���. ���� ���� �� �����͸� �о��.
 * @param hWnd ������ �ڵ�.
 * @param penMemory �ҷ��� �����͸� ������ ������.
 */
void FileManager::LoadFile(HWND hWnd, std::vector<PINFO>* penMemory)
{
    WCHAR file_name[256] = L"";
    OPENFILENAME OFN;
    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFilter = L"Guest Book(*.gb)\0*.gb";
    OFN.lpstrDefExt = L"gb";
    OFN.lpstrFile = file_name;
    OFN.nMaxFile = 256;
    OFN.lpstrInitialDir = L"C:\\Users\\siuugil\\Test\\2024_GuestBook_Team1\\savefile";
    OFN.Flags = OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&OFN)) {
        std::wifstream inFile(file_name);  /// ���� ����
        if (inFile.is_open()) {
            MessageBox(hWnd, L"������ �ҷ��Խ��ϴ�.", L"����", MB_OK);

            PINFO data;
            while (inFile >> data.lParam >> data.pWidth >> data.pColor >> data.pTime >> data.state >> data.bShape) {
                std::wstring line;
                while (std::getline(inFile, line)) {
                    AddFileToList(line);  /// ���� ����Ʈ�� �߰�
                }
            }

            inFile.close();
        }
        else {
            MessageBox(hWnd, L"���� ���⿡ �����߽��ϴ�.", L"����", MB_OK | MB_ICONERROR);
        }
    }
}



/**
 * @fn FileManager::SaveFileList()
 * @brief ���� ����Ʈ�� �ؽ�Ʈ ���Ϸ� ����.
 */
void FileManager::SaveFileList() {
    std::wofstream outFile(L"C:\\Users\\siuugil\\Test\\2024_GuestBook_Team1\\savefile\\fileList.txt");  // ���� ����Ʈ ����
    if (outFile.is_open()) {
        for (const auto& file : fileList) {
            outFile << file << std::endl;
        }
        outFile.close();
    }
}

/**
 * @fn FileManager::LoadFileList()
 * @brief ����� ���� ����Ʈ�� �ҷ��� UI�� �ݿ�.
 */
void FileManager::LoadFileList() {
    std::wifstream inFile(L"C:\\Users\\siuugil\\Test\\2024_GuestBook_Team1\\savefile\\fileList.txt");  // ���� ����Ʈ �ҷ�����
    if (inFile.is_open()) {
        std::wstring line;
        while (std::getline(inFile, line)) {
            fileList.push_back(line);  /// ���� ����Ʈ�� �߰�
        }
        inFile.close();
        UpdateFileListUI();  
    }
}


