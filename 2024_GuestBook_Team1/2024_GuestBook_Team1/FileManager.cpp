#include "FileManager.h"

FileManager FileManager::fileManager; //�ν��Ͻ� 

FileManager::FileManager() : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), m_hWnd(nullptr), isPanelVisible(false)
{} //�ʱ�ȭ�� ���� ������ 

FileManager::FileManager(HWND hWnd) : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), m_hWnd(hWnd) {}
// �ʱ�ȭ�� ���� ������

std::wstring FileManager::baseName = L"";
// ���� �̸��� ��ƿ� 

void FileManager::Panels(HWND hWnd) //���ϸ���Ʈ 
{
    hWnd = WndFunc::canvasWnd; // (�ӽ�)����Ϸ��� ������ �ڵ��� ĵ������������ �ڵ��� 
    hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE); // �θ��� �ν��Ͻ��� ������ 

    RECT rect; //����ü ���� 
    GetClientRect(WndFunc::canvasWnd, &rect); //ĵ������������ ����ü ������ ������ 


    /// ���̵� �޴� ��ġ �� ũ�� ����
    int top = 110;
    top += rect.top;
    int panelWidth = 300;
    int panelX = rect.right - panelWidth;
    int panelY = 400;

    /// ����Ʈ �ڽ��� ��ġ�� ũ�� ����
    int listBoxX = 10;
    int listBoxY = 40;
    int listBoxWidth = rect.right - 20;
    int listBoxHeight = rect.bottom - listBoxY - 10;

    //���ϸ���Ʈ �г� 
    hRightPanel = CreateWindowW(
        L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        rect.left, rect.top, rect.right, rect.bottom,
        hWnd, nullptr, hInst, nullptr);


    /// ���� �Ŵ��� �г� �ȿ� �ִ� �۾� ��("File Manager") 
    HWND hLabel = CreateWindowW(
        L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE,
        10, 10, rect.right - 20, 20,
        hRightPanel, nullptr, hInst, nullptr);

    // ���� �Ŵ��� ����Ʈ �ڽ� (�Ͼ� â)
    hFileListBox = CreateWindowW(
        L"LISTBOX", nullptr, WS_CHILD | WS_VSCROLL | LBS_NOTIFY | WS_VISIBLE,
        listBoxX, listBoxY, listBoxWidth, listBoxHeight,
        hRightPanel, nullptr, hInst, nullptr);

    // ���� �Ŵ��� ��ġ ���� 
    MoveWindow(hRightPanel, panelX, top, rect.right, rect.bottom, TRUE);

    // ���� �Ŵ����� �� ���� ������ (�ӽù���)
    SetWindowPos(hRightPanel, HWND_TOP, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

}

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
    // ����� ������ ��θ� ������
    wchar_t userProfilePath[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, userProfilePath))) {
        // ���� ��θ� ���� ���ߴٸ� �׳� ����
        MessageBox(nullptr, L"��� ������ �����߽��ϴ�.", L"����", MB_OK);
        return;
    }

    // �⺻ ������ ��ο� ������ ���� ��θ� �߰�
    std::wstring filePath = std::wstring(userProfilePath) + L"\\2024_GuestBook_T1\\file\\FileList.txt";

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
    // ����� ������ ��θ� ������
    wchar_t userProfilePath[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, userProfilePath))) {
        // ���� ��θ� ���� ���ߴٸ� �׳� ����
        return;
    }

    // �⺻ ������ ��ο� ���� ��θ� �߰�
    std::wstring filePath = std::wstring(userProfilePath) + L"\\2024_GuestBook_T1\\file\\FileList.txt";

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
    if (hFileListBox == nullptr)
    {
        return;
    }
    /// ���� ����Ʈ �ڽ� �ʱ�ȭ 
    SendMessage(hFileListBox, LB_RESETCONTENT, 0, 0);

    /// ���� ����Ʈ�� ���ϸ� �߰�
    for (const auto& file : savedFileList)
    {
        // LB_ADDSTRING API�� �߰� 
        SendMessage(hFileListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
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
    }

    /// FileNameW�� ���� �̸� ǥ��
    SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)baseName.c_str()); /// 2024_GuestBook_Team1�� �޽��� ����

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
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFilter = L"txt ����(*.txt)\0*.txt\0��� ����(*.*)\0*.*\0"; ///txt�� Ȯ���� ���� (�ӽ�) 
    OFN.lpstrFile = fileBuffer;
    OFN.nMaxFile = bufferSize;
    OFN.Flags = flags;

    return (flags & OFN_OVERWRITEPROMPT) ? GetSaveFileName(&OFN) : GetOpenFileName(&OFN);
}

// ������ ���ų� �����ϴ� �۾��� �ϴ� â�� �߰� �� (EX: �ٸ� �̸����� ����) 
// OFN_PATHMUSTEXIST: ��ΰ� ��ȿ�ؾ� ��.
// OFN_OVERWRITEPROMPT: ���� ���ϸ��� ������ ��� ����⸦ Ȯ��.
// OFN_FILEMUSTEXIST : ������ ������ �����ؾ� ��(���� ���� �� ���).
bool FileManager::HandleFileOperation(HWND hWnd, std::vector<PINFO>* penMemory, bool isSave) {


    DWORD flags = isSave ? (OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT) : (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
    WCHAR* filePath = isSave ? fileName : fileOpenName;

    if (ConfigureDialog(hWnd, flags, filePath, sizeof(fileName))) {
        if (isSave) {
            /// ���� Ȯ���ڰ� .txt�� ������ �߰�
            std::wstring path(filePath);
            if (path.find(L".txt") == std::wstring::npos) {
                path += L".txt";  /// .txt Ȯ���� �߰�
                wcscpy_s(filePath, sizeof(fileName) / sizeof(WCHAR), path.c_str());

            }
        }
        return isSave ? save(filePath, penMemory, hWnd) : load(filePath, penMemory, hWnd);
    }
    return false;
}


void FileManager::selectFileMode(int wmId, HWND s_hWnd, std::vector<PINFO>* penMemory)
{
    switch (wmId) {
    case SAVE:
    {
        *penMemory = Function::drawLInfo.pInfo;
        HandleFileOperation(s_hWnd, penMemory, true);
    }
    break;

    case LOAD:
    {
        HandleFileOperation(s_hWnd, penMemory, false);
        Function::drawLInfo.pInfo = *penMemory;

        // ȭ�� ����
        InvalidateRect(s_hWnd, NULL, TRUE);
        UpdateWindow(s_hWnd);
    }
    break;

    case SD_FILEMANAGER_BT:
    {
        // ���ϸŴ��� ���� / ǥ��
        if (!isPanelVisible)
        {
            if (hRightPanel == nullptr)
            {
                Panels(s_hWnd);
                LoadFileList();
            }
            // ���̵� �޴� ǥ��
            ShowWindow(hRightPanel, SW_SHOW);
            isPanelVisible = true;
        }
        else
        {
            // ���̵� �޴� ����
            ShowWindow(hRightPanel, SW_HIDE);
            isPanelVisible = false;
        }

        // ȭ�� ����
        InvalidateRect(s_hWnd, NULL, TRUE);
        UpdateWindow(s_hWnd);
    }
    break;
    }
}












