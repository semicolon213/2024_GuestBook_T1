/**
 * @file FileManager.cpp
 * @brief ���� �Ŵ��� ���� Ŭ���� �ܺ� ���� ����. ���� ����, �ҷ�����, ����Ʈ ���� ��� ����.
 * @author �����
 * @date 2024.09.20
 *
 * 
 * @todo UI������Ʈ�� �°� ��ǥ �� ��� ���ġ, �� �޸𸮿� �����Ͱ� ���� �ʴ� ���� ����
 * 
 */




#include "FileManager.h"




FileManager FileManager::fileManager;


/**
 * @fn FileManager::FileManager()
 * @brief FileManager �⺻ ������.
 */
FileManager::FileManager() : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), m_hWnd(nullptr), isPanelVisible(false)
{
    //LoadFileList();
}

/**
 * @fn FileManager::FileManager(HWND hWnd)
 * @brief HWND�� ���ڷ� �޴� ������.
 * @param hWnd ������ �ڵ�.
 */
FileManager::FileManager(HWND hWnd) : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), m_hWnd(hWnd){}




/**
 * @fn FileManager::InitializePanels(HWND hWnd)
 * @brief �г� �ʱ�ȭ. ������ �гΰ� ���� ����Ʈ �ڽ��� ����.
 * @param hWnd ������ �ڵ�.
 */
void FileManager::InitializePanels(HWND tWnd)
{
    hInst = (HINSTANCE)GetWindowLongPtr(tWnd, GWLP_HINSTANCE);

    static const int fixedWidth = 300; /// �г��� ������ �ʺ�
    static const int fixedHeight = 600; /// �г� ����


    /// ������ �г� ��ǥ (500, 50)
    hRightPanel = CreateWindowW(
        L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        500, 50, fixedWidth, fixedHeight,
        tWnd, nullptr, hInst, nullptr);


    /// ���� �Ŵ��� �۾� �� 
    HWND hLabel = CreateWindowW(
        L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE,
        10, 10, fixedWidth - 20, 20,
        hRightPanel, nullptr, hInst, nullptr);

    /// ����Ʈ �ڽ��� ��ġ�� ũ�� ����
    int listBoxX = 10;
    int listBoxY = 40;  /// �� �Ʒ��� ��ġ�ϰԲ�  ����
    int listBoxWidth = fixedWidth - 20;
    int listBoxHeight = fixedHeight - listBoxY - 10;  /// �Ʒ��� ����

    hFileListBox = CreateWindowW(
        L"LISTBOX", nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        listBoxX, listBoxY, listBoxWidth, listBoxHeight,
        hRightPanel, nullptr, hInst, nullptr);

    /*SetWindowPos(hRightPanel, HWND_TOP, 0, 0, 0, 0, 
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);*/ ///�г��� �� ������ ������(�ӽ� ����)

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
    MoveWindow(hRightPanel, width - fixedWidth, 0, fixedWidth, height, TRUE);
}

/**
 * @fn FileManager::AddFileToList(const std::wstring& fileName)
 * @brief ���� ����Ʈ�� ���� �߰� �� UI ������Ʈ.
 * @param fileName �߰��� ���� �̸�.
 */
void FileManager::AddFileToList(const std::wstring& fileName)
{
    ///���� �̸��� ����
    std::wstring baseName = fileName;
    size_t pos = fileName.find_last_of(L"\\/");
    if (pos != std::wstring::npos)
    {
        baseName = fileName.substr(pos + 1);
    }

    /// ���� �̸��� �̹� ����Ʈ�� �ִ��� Ȯ��
    if (std::find(savedFileList.begin(), savedFileList.end(), baseName) == savedFileList.end())
    {
        /// ����Ʈ�� �������� ������ �߰�
        savedFileList.push_back(baseName);
        UpdateFileListUI(); // UI ������Ʈ
    }
}

/**
 * @fn FileManager::UpdateFileListUI()
 * @brief ���� ����Ʈ UI ������Ʈ. ����Ʈ �ڽ��� ������.
 */
void FileManager::UpdateFileListUI()
{
    if (hFileListBox == nullptr)
    {
        // hFileListBox�� ���� �ʱ�ȭ���� �ʾ����Ƿ� �Լ��� �����մϴ�.
        return;
    }
    /// ���� ����Ʈ �ڽ� �ʱ�ȭ
    SendMessage(hFileListBox, LB_RESETCONTENT, 0, 0);

    /// ���� ����Ʈ�� ���ϸ� �߰�
    for (const auto& file : savedFileList)
    {
        SendMessage(hFileListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
    }
}



bool FileManager::HandleFileOperation(HWND hWnd, std::vector<PINFO>* penMemory, bool isSave) {
    if (penMemory == nullptr) {
        MessageBox(nullptr, L"penMemory�� null�Դϴ� - HandleFileOperation ȣ��", L"����", MB_OK);
        return false;
    }

    /// penMemory ũ�� Ȯ��
    std::wstring message = L"HandleFileOperation���� penMemory ũ��: " + std::to_wstring(penMemory->size());
    MessageBox(nullptr, message.c_str(), L"�����: HandleFileOperation", MB_OK);

    DWORD flags = isSave ? (OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT) : (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
    WCHAR* filePath = isSave ? fileName : fileOpenName;

    if (ConfigureDialog(hWnd, flags, filePath, sizeof(fileName))) {
        if (isSave) {
            /// ���� Ȯ���ڰ� .gb�� ������ �߰�
            std::wstring path(filePath);
            if (path.find(L".gb") == std::wstring::npos) {
                path += L".gb";  /// .gb Ȯ���� �߰�
                wcscpy_s(filePath, sizeof(fileName) / sizeof(WCHAR), path.c_str());

            }
        }
        return isSave ? save(filePath, penMemory) : load(filePath, penMemory, hWnd);
    }
    return false;
}




/**
 * @fn FileManager::save
 * @brief ������ �����ϴ� �Լ�.
 */
bool FileManager::save(const wchar_t* path, std::vector<PINFO>* penMemory) {



    
    /// penMemory ũ�� üũ ����
    if (!openForWrite(path)) {
        MessageBox(nullptr, L"���� ���忡 ���� �Ͽ����ϴ�.", L"Error", MB_OK);
        return false;
    }
    MessageBox(nullptr, L"������ ���忡 ���� �Ͽ����ϴ�.", L"Info", MB_OK);  // ���� ���� Ȯ��

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
    this->fs.close();
    AddFileToList(path);

    return true;
}
void FileManager::selectFileMode(int wmId, HWND cWnd, std::vector<PINFO>* penMemory)
{

    switch (wmId) {
    case SAVE:

    {

        *penMemory = Function::globalFunction.getDrawLInfo().pInfo;

        HandleFileOperation(cWnd, penMemory, true);
       
    }
    break;
    case LOAD:
    {
        HandleFileOperation(cWnd, penMemory, false);
        Function::globalFunction.getDrawLInfo().pInfo = *penMemory;


        //Function::globalFunction.setIsReplay(true);
        InvalidateRect(cWnd, NULL, TRUE);  // ȭ�� ����
        UpdateWindow(cWnd);  // ȭ�� ������Ʈ
        //Function::globalFunction.replay(cWnd);
    }
    break;

    case SD_FILEMANAGER_BT:
    {
        /// ���ϸŴ��� ���� / ǥ�� 
        if (!isPanelVisible)
        {
            
            if (hRightPanel == nullptr)
            {
                InitializePanels(cWnd);

                LoadFileList();
            }

            /// �θ� �������� ũ�� ��������
            RECT rect;
            GetClientRect(cWnd, &rect);

            int windowWidth = rect.right;
            int windowHeight = rect.bottom;
            int panelWidth = 300;
            int panelHeight = windowHeight;

            int panelX = windowWidth - panelWidth;
            int panelY = 0;

            /// ���̵� �޴� ��ġ �� ũ�� ����
            MoveWindow(hRightPanel, panelX, panelY, panelWidth, panelHeight, TRUE);

            /// ���̵� �޴� ǥ��
            ShowWindow(hRightPanel, SW_SHOW);
            isPanelVisible = true;
        }
        else
        {
            /// ���̵� �޴��� ����
            ShowWindow(hRightPanel, SW_HIDE);
            isPanelVisible = false;
        }

        /// ȭ�� ����
        InvalidateRect(cWnd, NULL, TRUE);
        UpdateWindow(cWnd);
    }
    break;


    }
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

    Function::globalFunction.getDrawLInfo().pInfo = *penMemory;

   
   // Function::globalFunction.replay(hWnd);  /// �׸��� ���
    
    InvalidateRect(hWnd, NULL, TRUE); /// ȭ�� ����
    UpdateWindow(hWnd); /// ȭ�� ������Ʈ
    

    return true;
}

/**
 * @fn FileManager::ConfigureDialog
 * @brief ���� ��ȭ���ڸ� �����ϴ� �Լ�.
 */
bool FileManager::ConfigureDialog(HWND hWnd, DWORD flags, WCHAR* fileBuffer, DWORD bufferSize) {
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFilter = L"GB ����(*.gb)\0*.gb\0��� ����(*.*)\0*.*\0"; ///GB�� Ȯ���� ���� 
    OFN.lpstrFile = fileBuffer;
    OFN.nMaxFile = bufferSize;
    OFN.Flags = flags;

    return (flags & OFN_OVERWRITEPROMPT) ? GetSaveFileName(&OFN) : GetOpenFileName(&OFN);
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

void FileManager::SaveFileList()
{
    std::wofstream ofs(L"FileList.txt", std::ios::out | std::ios::trunc);
    if (!ofs.is_open())
    {
        MessageBox(nullptr, L"���� ����Ʈ ���忡 �����߽��ϴ�.", L"����", MB_OK);
        return;
    }

    for (const auto& file : savedFileList)
    {
        ofs << file << std::endl;
    }

    ofs.close();
}

void FileManager::LoadFileList()
{
    std::wifstream ifs(L"C:\\Users\\tnrlf\\R_FileManager\\2024_GuestBook_Team1\\2024_GuestBook_Team1\\savefile\\FileList.txt"); ///��� ���� �ҰԿ� 
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
