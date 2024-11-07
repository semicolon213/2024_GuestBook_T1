#include "FileManager.h"

FileManager FileManager::fileManager; //�ν��Ͻ� 

std::wstring FileManager::baseName = L"";
// ���� �̸��� ��ƿ� 

// ����Ʈ �����ͷ� ConnExcel ��ü ����
std::unique_ptr<ConnExcel> connExcelFM = std::make_unique<ConnExcel>();


//���ϸ���Ʈ�� ������ �߰��ϴ� �Լ� 
void FileManager::AddFileToList(const std::wstring& fileName)
{
    ///1. baseName�̶�� ������ fileName�� ���ڿ��� �־���.
    std::wstring baseName = fileName;
    ///2. fileName�� ���ڿ����� ������ \ �Ǵ� /�� ã�� 
    size_t pos = fileName.find_last_of(L"\\/");
    ///3. 2���� ��찡 �ƴҶ�, pos + 1���� ���ڿ� ������ �߶� baseName�� ����
    if (pos != std::wstring::npos)
    {
        baseName = fileName.substr(pos + 1);
    }

    /// ���� �̸��� �̹� ����� ����Ʈ�� �ִ��� Ȯ��
    if (std::find(savedFileList.begin(), savedFileList.end(), baseName) == savedFileList.end())
    {
        /// ����Ʈ�� �������� ������ ����Ʈ�� �߰�
        savedFileList.push_back(baseName);
        //�� �� ���ϸ���Ʈ UI�� ������Ʈ 
        UpdateFileListUI();
    }
}
void FileManager::SaveFileList()
{
    // ����ȭ�� ��θ� ������
    wchar_t desktopPath[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktopPath))) {
        // ���� ��θ� ���� ���ߴٸ� �׳� ����
        MessageBox(nullptr, L"��� ������ �����߽��ϴ�.", L"����", MB_OK);
        return;
    }

    // ����ȭ�� ��ο� ������ ���� ��θ� �߰�
    std::wstring filePath = L"..\\file\\";

    // ���� ���� (��ο� ������ ������ ���� ������)
    std::wofstream ofs(filePath, std::ios::out | std::ios::trunc);
    if (!ofs.is_open())
    {
        MessageBox(nullptr, L"���� ����Ʈ ���忡 �����߽��ϴ�.", L"����", MB_OK);
        return;
    }

    // �ߺ��� ������ �����ϱ� ���� savedFileList�� ó���Ͽ� ����
    std::unordered_set<std::wstring> uniqueFiles(savedFileList.begin(), savedFileList.end());
    for (const auto& file : uniqueFiles)
    {
        ofs << file << std::endl;
    }

    ofs.close();
}

void FileManager::LoadFileList()
{
    // ����ȭ�� ��θ� ������
    wchar_t desktopPath[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktopPath))) {
        // ���� ��θ� ���� ���ߴٸ� �׳� ����
        return;
    }

    // ����ȭ�� ��ο� ���� ��θ� �߰�
    std::wstring filePath = L"..\\file\\";

    // ���� ����
    std::wifstream ifs(filePath);
    if (!ifs.is_open())
    {
        return;
    }

    std::wstring line;
    while (std::getline(ifs, line))
    {
        if (!line.empty())
        {
            savedFileList.push_back(line);
        }
    }

    ifs.close();

    UpdateFileListUI();
}



//���ϸ���Ʈ UI�� ������Ʈ ��
void FileManager::UpdateFileListUI()
{
    if (DW_FileManager::hListBox == nullptr)
    {
        return;
    }

    /// ���� ����Ʈ�� ���ϸ� �߰�
    for (const auto& file : savedFileList)
    {
        if (SendMessage(DW_FileManager::hListBox, LB_FINDSTRINGEXACT, -1, (LPARAM)file.c_str()) == LB_ERR) {
            // LB_ADDSTRING API�� �߰� 
            SendMessage(DW_FileManager::hListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
        }
    }
}



bool FileManager::save(const wchar_t* path, std::vector<PINFO>* penMemory, HWND hWnd) {

    if (!openForWrite(path)) {
        MessageBox(nullptr, L"���� ���忡 ���� �Ͽ����ϴ�.", L"Error", MB_OK);
        return false;
    }
    MessageBox(nullptr, L"���� ���忡 ���� �Ͽ����ϴ�.", L"Error", MB_OK);

    /// penMemory�� �����͸� ���Ͽ� ���
    for (const auto& i : *penMemory) {

        this->fs << i.lParam << ' '
            << i.pWidth << ' '
            << i.pColor << ' '
            << i.pTime << ' '
            << i.state << ' '
            << i.stampValue << ' '
            << i.bShape << std::endl;

        if (this->fs.fail()) {
            MessageBox(nullptr, L"���� ���� �� ���� �߻�", L"Error", MB_OK);
            break;
        }
    }

    /// ���� �̸��� ����
    baseName = path;
    size_t pos = baseName.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        baseName = baseName.substr(pos + 1);

        std::wstring visitorName = baseName; // �湮�� �̸��� wstring �������� ����
        connExcelFM->insertExcel(visitorName);
    }

    this->fs.close();
    AddFileToList(path);

    InvalidateRect(hWnd, NULL, TRUE); /// ȭ�� ����
    UpdateWindow(hWnd); /// ȭ�� ������Ʈ

    return true;
}

/**
 * @fn FileManager::load
 * @brief ������ �ε��ϴ� �Լ�.
 */
bool FileManager::load(const wchar_t* path, std::vector<PINFO>* penMemory, HWND hWnd) {
    if (!openForRead(path)) {
        MessageBox(nullptr, L"���� ���⿡ �����߽��ϴ�.", L"Error", MB_OK);
        return false;
    }
    MessageBox(nullptr, L"������ ���������� ���Ƚ��ϴ�.", L"Info", MB_OK);

    penMemory->clear();
    PINFO pen_info;

    /// ���Ͽ��� �����͸� �о�� penMemory�� �߰�
    while (this->fs >> pen_info.lParam
        >> pen_info.pWidth
        >> pen_info.pColor
        >> pen_info.pTime
        >> pen_info.state
        >> pen_info.stampValue
        >> pen_info.bShape) {
        penMemory->push_back(pen_info);
    }

    this->fs.close(); /// ������ ����
    AddFileToList(path);  /// ���� ����Ʈ�� �߰�
    UpdateFileListUI();

    // Function::drawLInfo.pInfo = *penMemory;

     /// ���� �̸��� ����
    baseName = path;
    size_t pos = baseName.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        baseName = baseName.substr(pos + 1);
    }

    /// FileNameW�� ���� �̸� ǥ��
    SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)baseName.c_str());

    InvalidateRect(hWnd, NULL, TRUE); /// ȭ�� ����
    UpdateWindow(hWnd); /// ȭ�� ������Ʈ


    return true;
}
/**
 * @fn FileManager::openForWrite
 * @brief ������ ���� ���� ���� �Լ�.
 */
bool FileManager::openForWrite(const wchar_t* path) {
    fs.open(path, std::ios::out | std::ios::trunc);
    return !fs.fail();
}

/**
 * @fn FileManager::openForRead
 * @brief ������ �б� ���� ���� �Լ�.
 */
bool FileManager::openForRead(const wchar_t* path) {
    fs.open(path, std::ios::in);
    return !fs.fail();
}

// ���� ��ȭ���ڸ� ���� �Լ� 
bool FileManager::ConfigureDialog(HWND hWnd, DWORD flags, WCHAR* fileBuffer, DWORD bufferSize) {
    // ����ȭ�� ��� ��������
    wchar_t desktopPath[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktopPath))) {
        MessageBox(nullptr, L"��� ������ �����߽��ϴ�.", L"����", MB_OK);
        return false;
    }

    // "file" ���� ��� ����
    std::wstring initialDir = L"..\\file\\";

    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFile = fileBuffer;
    OFN.nMaxFile = bufferSize;
    OFN.lpstrInitialDir = initialDir.c_str(); // �ʱ� ���丮 ����
    OFN.Flags = flags;

    return (flags & OFN_OVERWRITEPROMPT) ? GetSaveFileName(&OFN) : GetOpenFileName(&OFN);
}

/// ������ ���ų� �����ϴ� �۾��� �ϴ� â�� �߰� �� (EX: �ٸ� �̸����� ����) 
/// OFN_PATHMUSTEXIST: ��ΰ� ��ȿ�ؾ� ��.
/// OFN_OVERWRITEPROMPT: ���� ���ϸ��� ������ ��� ����⸦ Ȯ��.
/// OFN_FILEMUSTEXIST : ������ ������ �����ؾ� ��(���� ���� �� ���).
bool FileManager::HandleFileOperation(HWND hWnd, std::vector<PINFO>* penMemory, bool isSave) {


    DWORD flags = isSave ? (OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT) : (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
    WCHAR* filePath = isSave ? fileName : fileOpenName;

    /// ��ȭ���ڰ� ��ҵǸ� false�� ��ȯ�ϵ��� ����
    if (!ConfigureDialog(hWnd, flags, filePath, sizeof(fileName))) {
        //DW_SideMenu::penMemory->clear();
        SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_CLEAR_BT, 0);
        SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)L"�̸� ����");
        return false;  // ��ȭ���ڰ� ��ҵǸ� �ٷ� ����
    }

    // ConfigureDialog�� ���������� �Ϸ�� ��쿡�� ���� �Ǵ� �ε� ����
    return isSave ? save(filePath, penMemory, hWnd) : load(filePath, penMemory, hWnd);
}


void FileManager::selectFileMode(int wmId, HWND s_hWnd, std::vector<PINFO>* penMemory)
{
    switch (wmId) {
    case SAVE:
    {
        *penMemory = Function::drawLInfo.pInfo;
        // penMemory�� ������� ���� ��쿡�� �޽��� ����
        HandleFileOperation(s_hWnd, penMemory, true);
        SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_CLEAR_BT, 0);
        SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)L"�̸� ����");
    }
    break;

    case LOAD:
    {
        HandleFileOperation(s_hWnd, penMemory, false);
        Function::drawLInfo.pInfo = *penMemory;

        // penMemory�� ������� ���� ��쿡�� �޽��� ����
        if (!penMemory->empty())
        {
            SendMessage(WndFunc::toolWnd, WM_COMMAND, TL_PLAY_BT, 0); // �߰�
        }

        if (IsWindowVisible(WndFunc::fileManager) || IsWindowVisible(WndFunc::sideWnd))
        {
            ShowWindow(WndFunc::fileManager, SW_HIDE); // ���� ������ �ݱ�
            ShowWindow(WndFunc::sideWnd, SW_HIDE); // ���� ������ �ݱ�
            DW_NameBar::sideMenu.toggleState = false;
            InvalidateRect(WndFunc::nameWnd, NULL, TRUE);
        }

        /// ȭ�� ����
        InvalidateRect(s_hWnd, NULL, TRUE);
        UpdateWindow(s_hWnd);
    }
    break;

    case SD_FILEMANAGER_BT:
    {
        /// MessageBox(nullptr, DW_FileManager::filePath.c_str(), L"���� ���", MB_OK); // ��� ���

        if (IsWindowVisible(WndFunc::fileManager) || IsWindowVisible(WndFunc::sideWnd))
        {
            ShowWindow(WndFunc::fileManager, SW_HIDE); // ���� ������ �ݱ�
            ShowWindow(WndFunc::sideWnd, SW_HIDE); // ���� ������ �ݱ�
            DW_NameBar::sideMenu.toggleState = false;
            InvalidateRect(WndFunc::nameWnd, NULL, TRUE);

        }
        /// filePath�� ����� ������ ��� �ε�
        if (load(DW_FileManager::filePath.c_str(), penMemory, s_hWnd)) {
            Function::drawLInfo.pInfo = *penMemory;

            /// ���� �̸��� ����
            baseName = DW_FileManager::filePath;
            size_t pos = baseName.find_last_of(L"\\/");
            if (pos != std::wstring::npos) {
                baseName = baseName.substr(pos + 1);
            }

            /// FileNameW�� ���� �̸� ǥ��
            SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)baseName.c_str()); /// 2024_GuestBook_Team1�� �޽��� ����
            SendMessage(WndFunc::toolWnd, WM_COMMAND, TL_PLAY_BT, 0); /// �߰�

            /// ȭ�� ����
            InvalidateRect(s_hWnd, NULL, TRUE);
            UpdateWindow(s_hWnd);
        }
    }
    break;
    }
}












