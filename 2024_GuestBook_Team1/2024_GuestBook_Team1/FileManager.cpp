#include "FileManager.h"

FileManager FileManager::fileManager; //인스턴스 

FileManager::FileManager() : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), m_hWnd(nullptr), isPanelVisible(false)
{} //초기화를 위한 생성자 

FileManager::FileManager(HWND hWnd) : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), m_hWnd(hWnd) {}
// 초기화를 위한 생성자

std::wstring FileManager::baseName = L"";
// 파일 이름을 담아요 

void FileManager::Panels(HWND hWnd) //파일리스트 
{
    hWnd = WndFunc::canvasWnd; // (임시)사용하려는 윈도우 핸들은 캔버스윈도우의 핸들임 
    hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE); // 부모의 인스턴스를 가져옴 

    RECT rect; //구조체 선언 
    GetClientRect(WndFunc::canvasWnd, &rect); //캔버스윈도우의 구조체 영역을 가져옴 


    /// 사이드 메뉴 위치 및 크기 설정
    int top = 110;
    top += rect.top;
    int panelWidth = 300;
    int panelX = rect.right - panelWidth;
    int panelY = 400;

    /// 리스트 박스의 위치와 크기 설정
    int listBoxX = 10;
    int listBoxY = 40;
    int listBoxWidth = rect.right - 20;
    int listBoxHeight = rect.bottom - listBoxY - 10;

    //파일리스트 패널 
    hRightPanel = CreateWindowW(
        L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        rect.left, rect.top, rect.right, rect.bottom,
        hWnd, nullptr, hInst, nullptr);


    /// 파일 매니저 패널 안에 있는 글씨 라벨("File Manager") 
    HWND hLabel = CreateWindowW(
        L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE,
        10, 10, rect.right - 20, 20,
        hRightPanel, nullptr, hInst, nullptr);

    // 파일 매니저 리스트 박스 (하얀 창)
    hFileListBox = CreateWindowW(
        L"LISTBOX", nullptr, WS_CHILD | WS_VSCROLL | LBS_NOTIFY | WS_VISIBLE,
        listBoxX, listBoxY, listBoxWidth, listBoxHeight,
        hRightPanel, nullptr, hInst, nullptr);

    // 파일 매니저 위치 조절 
    MoveWindow(hRightPanel, panelX, top, rect.right, rect.bottom, TRUE);

    // 파일 매니저를 맨 위로 가져옴 (임시방편)
    SetWindowPos(hRightPanel, HWND_TOP, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

}

//파일리스트에 파일을 추가하는 함수 
void FileManager::AddFileToList(const std::wstring& fileName)
{
    ///1. baseName이라는 변수에 fileName의 문자열을 넣어줌.
    std::wstring baseName = fileName;
    ///2. fileName의 문자열에서 마지막 \ 또는 /을 찾음 
    size_t pos = fileName.find_last_of(L"\\/");
    ///3. 2번의 경우가 아닐때, pos + 1부터 문자열 끝까지 잘라서 baseName에 저장
    if (pos != std::wstring::npos)
    {
        baseName = fileName.substr(pos + 1);
    }

    /// 파일 이름이 이미 저장된 리스트에 있는지 확인
    if (std::find(savedFileList.begin(), savedFileList.end(), baseName) == savedFileList.end())
    {
        /// 리스트에 존재하지 않으면 리스트에 추가
        savedFileList.push_back(baseName);
        //그 후 파일리스트 UI를 업데이트 
        UpdateFileListUI();
    }
}
void FileManager::SaveFileList()
{
    // 사용자 프로필 경로를 가져옴
    wchar_t userProfilePath[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, userProfilePath))) {
        // 만약 경로를 얻지 못했다면 그냥 종료
        MessageBox(nullptr, L"경로 설정에 실패했습니다.", L"오류", MB_OK);
        return;
    }

    // 기본 프로필 경로에 저장할 파일 경로를 추가
    std::wstring filePath = std::wstring(userProfilePath) + L"\\2024_GuestBook_T1\\file\\FileList.txt";

    // 파일 열기 (경로에 파일이 없으면 새로 생성됨)
    std::wofstream ofs(filePath, std::ios::out | std::ios::trunc);
    if (!ofs.is_open())
    {
        MessageBox(nullptr, L"파일 리스트 저장에 실패했습니다.", L"오류", MB_OK);
        return;
    }

    // 중복된 파일을 방지하기 위해 savedFileList를 처리하여 저장
    std::unordered_set<std::wstring> uniqueFiles(savedFileList.begin(), savedFileList.end());
    for (const auto& file : uniqueFiles)
    {
        ofs << file << std::endl;
    }

    ofs.close();
}

void FileManager::LoadFileList()
{
    // 사용자 프로필 경로를 가져옴
    wchar_t userProfilePath[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, userProfilePath))) {
        // 만약 경로를 얻지 못했다면 그냥 종료
        return;
    }

    // 기본 프로필 경로에 파일 경로를 추가
    std::wstring filePath = std::wstring(userProfilePath) + L"\\2024_GuestBook_T1\\file\\FileList.txt";

    // 파일 열기
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


//파일리스트 UI를 업데이트 함
void FileManager::UpdateFileListUI()
{
    if (hFileListBox == nullptr)
    {
        return;
    }
    /// 기존 리스트 박스 초기화 
    SendMessage(hFileListBox, LB_RESETCONTENT, 0, 0);

    /// 파일 리스트에 파일명 추가
    for (const auto& file : savedFileList)
    {
        // LB_ADDSTRING API로 추가 
        SendMessage(hFileListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
    }
}



bool FileManager::save(const wchar_t* path, std::vector<PINFO>* penMemory, HWND hWnd) {

    if (!openForWrite(path)) {
        MessageBox(nullptr, L"파일 저장에 실패 하였습니다.", L"Error", MB_OK);
        return false;
    }
    MessageBox(nullptr, L"파일 저장에 성공 하였습니다.", L"Error", MB_OK);

    /// penMemory의 데이터를 파일에 기록
    for (const auto& i : *penMemory) {

        this->fs << i.lParam << ' '
            << i.pWidth << ' '
            << i.pColor << ' '
            << i.pTime << ' '
            << i.state << ' '
            << i.bShape << std::endl;

        if (this->fs.fail()) {
            MessageBox(nullptr, L"파일 쓰기 중 오류 발생", L"Error", MB_OK);
            break;
        }
    }

    /// 파일 이름만 추출
    baseName = path;
    size_t pos = baseName.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        baseName = baseName.substr(pos + 1);
    }

    /// FileNameW에 파일 이름 표시
    SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)baseName.c_str()); /// 2024_GuestBook_Team1로 메시지 전달

    this->fs.close();
    AddFileToList(path);

    InvalidateRect(hWnd, NULL, TRUE); /// 화면 갱신
    UpdateWindow(hWnd); /// 화면 업데이트

    return true;
}

/**
 * @fn FileManager::load
 * @brief 파일을 로드하는 함수.
 */
bool FileManager::load(const wchar_t* path, std::vector<PINFO>* penMemory, HWND hWnd) {
    if (!openForRead(path)) {
        MessageBox(nullptr, L"파일 열기에 실패했습니다.", L"Error", MB_OK);
        return false;
    }
    MessageBox(nullptr, L"파일이 성공적으로 열렸습니다.", L"Info", MB_OK);

    penMemory->clear();
    PINFO pen_info;

    /// 파일에서 데이터를 읽어와 penMemory에 추가
    while (this->fs >> pen_info.lParam
        >> pen_info.pWidth
        >> pen_info.pColor
        >> pen_info.pTime
        >> pen_info.state
        >> pen_info.bShape) {
        penMemory->push_back(pen_info);
    }

    this->fs.close(); /// 파일을 닫음
    AddFileToList(path);  /// 파일 리스트에 추가
    UpdateFileListUI();

    // Function::drawLInfo.pInfo = *penMemory;

     /// 파일 이름만 추출
    baseName = path;
    size_t pos = baseName.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        baseName = baseName.substr(pos + 1);
    }

    /// FileNameW에 파일 이름 표시
    SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)baseName.c_str());

    InvalidateRect(hWnd, NULL, TRUE); /// 화면 갱신
    UpdateWindow(hWnd); /// 화면 업데이트


    return true;
}
/**
 * @fn FileManager::openForWrite
 * @brief 파일을 쓰기 모드로 여는 함수.
 */
bool FileManager::openForWrite(const wchar_t* path) {
    fs.open(path, std::ios::out | std::ios::trunc);
    return !fs.fail();
}

/**
 * @fn FileManager::openForRead
 * @brief 파일을 읽기 모드로 여는 함수.
 */
bool FileManager::openForRead(const wchar_t* path) {
    fs.open(path, std::ios::in);
    return !fs.fail();
}

// 파일 대화상자를 여는 함수 
bool FileManager::ConfigureDialog(HWND hWnd, DWORD flags, WCHAR* fileBuffer, DWORD bufferSize) {
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFilter = L"txt 파일(*.txt)\0*.txt\0모든 파일(*.*)\0*.*\0"; ///txt로 확장자 설정 (임시) 
    OFN.lpstrFile = fileBuffer;
    OFN.nMaxFile = bufferSize;
    OFN.Flags = flags;

    return (flags & OFN_OVERWRITEPROMPT) ? GetSaveFileName(&OFN) : GetOpenFileName(&OFN);
}

// 파일을 열거나 저장하는 작업을 하는 창을 뜨게 함 (EX: 다른 이름으로 저장) 
// OFN_PATHMUSTEXIST: 경로가 유효해야 함.
// OFN_OVERWRITEPROMPT: 같은 파일명이 존재할 경우 덮어쓰기를 확인.
// OFN_FILEMUSTEXIST : 파일이 실제로 존재해야 함(파일 열기 시 사용).
bool FileManager::HandleFileOperation(HWND hWnd, std::vector<PINFO>* penMemory, bool isSave) {


    DWORD flags = isSave ? (OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT) : (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
    WCHAR* filePath = isSave ? fileName : fileOpenName;

    if (ConfigureDialog(hWnd, flags, filePath, sizeof(fileName))) {
        if (isSave) {
            /// 파일 확장자가 .txt가 없으면 추가
            std::wstring path(filePath);
            if (path.find(L".txt") == std::wstring::npos) {
                path += L".txt";  /// .txt 확장자 추가
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

        // 화면 갱신
        InvalidateRect(s_hWnd, NULL, TRUE);
        UpdateWindow(s_hWnd);
    }
    break;

    case SD_FILEMANAGER_BT:
    {
        // 파일매니저 숨김 / 표시
        if (!isPanelVisible)
        {
            if (hRightPanel == nullptr)
            {
                Panels(s_hWnd);
                LoadFileList();
            }
            // 사이드 메뉴 표시
            ShowWindow(hRightPanel, SW_SHOW);
            isPanelVisible = true;
        }
        else
        {
            // 사이드 메뉴 숨김
            ShowWindow(hRightPanel, SW_HIDE);
            isPanelVisible = false;
        }

        // 화면 갱신
        InvalidateRect(s_hWnd, NULL, TRUE);
        UpdateWindow(s_hWnd);
    }
    break;
    }
}












