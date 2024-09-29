/**
 * @file FileManager.cpp
 * @brief 파일 매니저 관련 클래스 외부 정의 파일. 파일 저장, 불러오기, 리스트 관리 기능 제공.
 * @author 김수길
 * @date 2024.09.20
 *
 * 
 * @todo UI업데이트에 맞게 좌표 및 기능 재배치, 펜 메모리에 데이터가 들어가지 않는 오류 수정
 * 
 */




#include "FileManager.h"




FileManager FileManager::fileManager;


/**
 * @fn FileManager::FileManager()
 * @brief FileManager 기본 생성자.
 */
FileManager::FileManager() : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), m_hWnd(nullptr), isPanelVisible(false)
{
    //LoadFileList();
}

/**
 * @fn FileManager::FileManager(HWND hWnd)
 * @brief HWND를 인자로 받는 생성자.
 * @param hWnd 윈도우 핸들.
 */
FileManager::FileManager(HWND hWnd) : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), m_hWnd(hWnd){}




/**
 * @fn FileManager::InitializePanels(HWND hWnd)
 * @brief 패널 초기화. 오른쪽 패널과 파일 리스트 박스를 생성.
 * @param hWnd 윈도우 핸들.
 */
void FileManager::InitializePanels(HWND tWnd)
{
    hInst = (HINSTANCE)GetWindowLongPtr(tWnd, GWLP_HINSTANCE);

    static const int fixedWidth = 300; /// 패널의 고정된 너비
    static const int fixedHeight = 600; /// 패널 높이


    /// 오른쪽 패널 좌표 (500, 50)
    hRightPanel = CreateWindowW(
        L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        500, 50, fixedWidth, fixedHeight,
        tWnd, nullptr, hInst, nullptr);


    /// 파일 매니저 글씨 라벨 
    HWND hLabel = CreateWindowW(
        L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE,
        10, 10, fixedWidth - 20, 20,
        hRightPanel, nullptr, hInst, nullptr);

    /// 리스트 박스의 위치와 크기 설정
    int listBoxX = 10;
    int listBoxY = 40;  /// 라벨 아래에 위치하게끔  설정
    int listBoxWidth = fixedWidth - 20;
    int listBoxHeight = fixedHeight - listBoxY - 10;  /// 아래쪽 여백

    hFileListBox = CreateWindowW(
        L"LISTBOX", nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        listBoxX, listBoxY, listBoxWidth, listBoxHeight,
        hRightPanel, nullptr, hInst, nullptr);

    /*SetWindowPos(hRightPanel, HWND_TOP, 0, 0, 0, 0, 
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);*/ ///패널을 맨 앞으로 가져옴(임시 방편)

}

/**
 * @fn FileManager::ResizePanels(HWND hWnd, LPARAM lParam)
 * @brief 윈도우 크기에 따라 패널 크기를 조정.
 * @param hWnd 윈도우 핸들.
 * @param lParam 크기 파라미터.
 */
void FileManager::ResizePanels(HWND hWnd, LPARAM lParam)
{
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    static const int fixedWidth = 300;

    /// 패널 크기 조정
    MoveWindow(hRightPanel, width - fixedWidth, 0, fixedWidth, height, TRUE);
}

/**
 * @fn FileManager::AddFileToList(const std::wstring& fileName)
 * @brief 파일 리스트에 파일 추가 후 UI 업데이트.
 * @param fileName 추가할 파일 이름.
 */
void FileManager::AddFileToList(const std::wstring& fileName)
{
    ///파일 이름만 추출
    std::wstring baseName = fileName;
    size_t pos = fileName.find_last_of(L"\\/");
    if (pos != std::wstring::npos)
    {
        baseName = fileName.substr(pos + 1);
    }

    /// 파일 이름이 이미 리스트에 있는지 확인
    if (std::find(savedFileList.begin(), savedFileList.end(), baseName) == savedFileList.end())
    {
        /// 리스트에 존재하지 않으면 추가
        savedFileList.push_back(baseName);
        UpdateFileListUI(); // UI 업데이트
    }
}

/**
 * @fn FileManager::UpdateFileListUI()
 * @brief 파일 리스트 UI 업데이트. 리스트 박스를 갱신함.
 */
void FileManager::UpdateFileListUI()
{
    if (hFileListBox == nullptr)
    {
        // hFileListBox가 아직 초기화되지 않았으므로 함수를 종료합니다.
        return;
    }
    /// 기존 리스트 박스 초기화
    SendMessage(hFileListBox, LB_RESETCONTENT, 0, 0);

    /// 파일 리스트에 파일명 추가
    for (const auto& file : savedFileList)
    {
        SendMessage(hFileListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
    }
}



bool FileManager::HandleFileOperation(HWND hWnd, std::vector<PINFO>* penMemory, bool isSave) {
    if (penMemory == nullptr) {
        MessageBox(nullptr, L"penMemory가 null입니다 - HandleFileOperation 호출", L"오류", MB_OK);
        return false;
    }

    /// penMemory 크기 확인
    std::wstring message = L"HandleFileOperation에서 penMemory 크기: " + std::to_wstring(penMemory->size());
    MessageBox(nullptr, message.c_str(), L"디버깅: HandleFileOperation", MB_OK);

    DWORD flags = isSave ? (OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT) : (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
    WCHAR* filePath = isSave ? fileName : fileOpenName;

    if (ConfigureDialog(hWnd, flags, filePath, sizeof(fileName))) {
        if (isSave) {
            /// 파일 확장자가 .gb가 없으면 추가
            std::wstring path(filePath);
            if (path.find(L".gb") == std::wstring::npos) {
                path += L".gb";  /// .gb 확장자 추가
                wcscpy_s(filePath, sizeof(fileName) / sizeof(WCHAR), path.c_str());

            }
        }
        return isSave ? save(filePath, penMemory) : load(filePath, penMemory, hWnd);
    }
    return false;
}




/**
 * @fn FileManager::save
 * @brief 파일을 저장하는 함수.
 */
bool FileManager::save(const wchar_t* path, std::vector<PINFO>* penMemory) {



    
    /// penMemory 크기 체크 제거
    if (!openForWrite(path)) {
        MessageBox(nullptr, L"파일 저장에 실패 하였습니다.", L"Error", MB_OK);
        return false;
    }
    MessageBox(nullptr, L"파일이 저장에 성공 하였습니다.", L"Info", MB_OK);  // 파일 열기 확인

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
        InvalidateRect(cWnd, NULL, TRUE);  // 화면 갱신
        UpdateWindow(cWnd);  // 화면 업데이트
        //Function::globalFunction.replay(cWnd);
    }
    break;

    case SD_FILEMANAGER_BT:
    {
        /// 파일매니저 숨김 / 표시 
        if (!isPanelVisible)
        {
            
            if (hRightPanel == nullptr)
            {
                InitializePanels(cWnd);

                LoadFileList();
            }

            /// 부모 윈도우의 크기 가져오기
            RECT rect;
            GetClientRect(cWnd, &rect);

            int windowWidth = rect.right;
            int windowHeight = rect.bottom;
            int panelWidth = 300;
            int panelHeight = windowHeight;

            int panelX = windowWidth - panelWidth;
            int panelY = 0;

            /// 사이드 메뉴 위치 및 크기 설정
            MoveWindow(hRightPanel, panelX, panelY, panelWidth, panelHeight, TRUE);

            /// 사이드 메뉴 표시
            ShowWindow(hRightPanel, SW_SHOW);
            isPanelVisible = true;
        }
        else
        {
            /// 사이드 메뉴를 숨김
            ShowWindow(hRightPanel, SW_HIDE);
            isPanelVisible = false;
        }

        /// 화면 갱신
        InvalidateRect(cWnd, NULL, TRUE);
        UpdateWindow(cWnd);
    }
    break;


    }
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

    Function::globalFunction.getDrawLInfo().pInfo = *penMemory;

   
   // Function::globalFunction.replay(hWnd);  /// 그리기 재생
    
    InvalidateRect(hWnd, NULL, TRUE); /// 화면 갱신
    UpdateWindow(hWnd); /// 화면 업데이트
    

    return true;
}

/**
 * @fn FileManager::ConfigureDialog
 * @brief 파일 대화상자를 설정하는 함수.
 */
bool FileManager::ConfigureDialog(HWND hWnd, DWORD flags, WCHAR* fileBuffer, DWORD bufferSize) {
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFilter = L"GB 파일(*.gb)\0*.gb\0모든 파일(*.*)\0*.*\0"; ///GB로 확장자 설정 
    OFN.lpstrFile = fileBuffer;
    OFN.nMaxFile = bufferSize;
    OFN.Flags = flags;

    return (flags & OFN_OVERWRITEPROMPT) ? GetSaveFileName(&OFN) : GetOpenFileName(&OFN);
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

void FileManager::SaveFileList()
{
    std::wofstream ofs(L"FileList.txt", std::ios::out | std::ios::trunc);
    if (!ofs.is_open())
    {
        MessageBox(nullptr, L"파일 리스트 저장에 실패했습니다.", L"오류", MB_OK);
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
    std::wifstream ifs(L"C:\\Users\\tnrlf\\R_FileManager\\2024_GuestBook_Team1\\2024_GuestBook_Team1\\savefile\\FileList.txt"); ///경로 수정 할게요 
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
