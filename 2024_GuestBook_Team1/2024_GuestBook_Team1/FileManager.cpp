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

std::vector<std::wstring> fileList;

/**
 * @fn FileManager::FileManager()
 * @brief FileManager 기본 생성자.
 */
FileManager::FileManager() : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), hWnd(nullptr){}

/**
 * @fn FileManager::FileManager(HWND hWnd)
 * @brief HWND를 인자로 받는 생성자.
 * @param hWnd 윈도우 핸들.
 */
FileManager::FileManager(HWND hWnd) : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), hWnd(hWnd){}

/**
 * @fn FileManager::InitializePanels(HWND hWnd)
 * @brief 패널 초기화. 오른쪽 패널과 파일 리스트 박스를 생성.
 * @param hWnd 윈도우 핸들.
 */
void FileManager::InitializePanels(HWND hWnd)
{
    static const int fixedWidth = 300; // 오른쪽 패널의 고정된 너비

    /// 오른쪽 패널 생성
    getInstance().hRightPanel = CreateWindowW(L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, fixedWidth, 600, hWnd, (HMENU)ID_RIGHT_PANEL, GetModuleHandle(NULL), NULL);

    /// 파일 리스트 박스 생성
    getInstance().hFileListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, fixedWidth - 20, 750, getInstance().hRightPanel, (HMENU)ID_FILE_LIST, GetModuleHandle(NULL), NULL);
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
    MoveWindow(getInstance().hRightPanel, width - fixedWidth, 0, fixedWidth, height, TRUE);
}

/**
 * @fn FileManager::AddFileToList(const std::wstring& fileName)
 * @brief 파일 리스트에 파일 추가 후 UI 업데이트.
 * @param fileName 추가할 파일 이름.
 */
void FileManager::AddFileToList(const std::wstring& fileName)
{
    fileList.push_back(fileName);
    UpdateFileListUI();
}

/**
 * @fn FileManager::UpdateFileListUI()
 * @brief 파일 리스트 UI 업데이트. 리스트 박스를 갱신함.
 */
void FileManager::UpdateFileListUI()
{
    /// 리스트 박스 비우기
    SendMessage(getInstance().hFileListBox, LB_RESETCONTENT, 0, 0);

    /// 파일 리스트 추가
    for (const auto& file : fileList)
    {
        SendMessage(getInstance().hFileListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
    }
}


/*파일 매니저 함수*/
/**
* @fn FileManager::SaveFile(HWND hWnd, std::vector<PINFO>* penMemory)
* @brief 파일 저장 기능. 저장 버튼 클릭 시 호출됨.
* @param hWnd 윈도우 핸들.
* @param penMemory 저장할 데이터를 포함하는 포인터.
*/

/**
 * @fn FileManager::FileManager_Open()
 * @brief 패널 열림/닫힘 상태를 토글함.
 */
void FileManager::FileManager_Open()
{
    is_open_ = !is_open_;  /// 열림/닫힘 상태 토글

    if (is_open_) {
        ShowWindow(getInstance().hRightPanel, SW_SHOW);  /// 패널 표시
    }
    else {
        ShowWindow(getInstance().hRightPanel, SW_HIDE);  /// 패널 숨기기
    }
    
    
}
/**
 * @fn FileManager::SaveFile(HWND hWnd, std::vector<PINFO>* penMemory)
 * @brief 파일 저장 기능. 저장 버튼 클릭 시 호출됨.
 * @param hWnd 윈도우 핸들.
 * @param penMemory 저장할 데이터를 포함하는 포인터.
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
    OFN.lpstrInitialDir = L"C:\\Users\\tnrlf\\FileManagerTest\\2024_GuestBook_Team1\\savefile";
    OFN.Flags = OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&OFN)) {
        std::wofstream outFile(file_name);  // 파일 열기

        if (!outFile.is_open()) {
            MessageBox(hWnd, L"파일을 열 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
            return;
        }

        // 데이터 파일에 저장
        for (const auto& data : *penMemory) {
            outFile << data.lParam << L' '
                << data.pWidth << L' '
                << data.pColor << L' '
                << data.pTime << L' '
                << data.state << L' '
                << data.bShape << std::endl;
        }

        outFile.close();
        AddFileToList(file_name);  // 파일 리스트에 추가
    }
}



/*
 * @fn FileManager::LoadFile(HWND hWnd, std::vector<PINFO>* penMemory)
 * @brief 파일 불러오기 기능. 파일 선택 후 데이터를 읽어옴.
 * @param hWnd 윈도우 핸들.
 * @param penMemory 불러올 데이터를 저장할 포인터.
 */
void FileManager::LoadFile(HWND hWnd, std::vector<PINFO> *penMemory) {
    WCHAR file_name[256] = L"";
    OPENFILENAME OFN;
    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd;
    OFN.lpstrFilter = L"Guest Book(*.gb)\0*.gb";
    OFN.lpstrDefExt = L"gb";
    OFN.lpstrFile = file_name;
    OFN.nMaxFile = 256;
    OFN.lpstrInitialDir = L"C:\\Users\\tnrlf\\FileManagerTest\\2024_GuestBook_Team1\\savefile";
    OFN.Flags = OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&OFN)) {
        std::wifstream inFile(file_name);  /// 파일 열기

        if (!inFile.is_open()) {
            MessageBox(hWnd, L"파일 열기에 실패했습니다.", L"오류", MB_OK | MB_ICONERROR);
            return;
        }

        PINFO data;
        while (inFile >> data.lParam >> data.pWidth >> data.pColor >> data.pTime >> data.state >> data.bShape) {
            penMemory->push_back(data);
        }

        inFile.close();
        AddFileToList(file_name);  /// 파일 리스트에 추가
        UpdateFileListUI();  
    }
}


/**
 * @fn FileManager::SaveFileList()
 * @brief 파일 리스트를 텍스트 파일로 저장.
 */
void FileManager::SaveFileList() {
    std::wstring filePath = L"C:\\Users\\tnrlf\\savefile\\fileList.txt";

    std::wofstream outFile(filePath);  /// 파일 저장

    if (outFile.is_open()) {
        for (const auto& file : fileList) {
            outFile << file << std::endl;
        }
        
    }
    else {
        MessageBox(hWnd, L"파일을 열 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);  // 파일 열기 오류 처리
    }
    outFile.close();
}

/**
 * @fn FileManager::LoadFileList()
 * @brief 저장된 파일 리스트를 불러와 UI에 반영.
 */
void FileManager::LoadFileList() {
    std::wstring filePath = L"C:\\Users\\tnrlf\\savefile\\fileList.txt";
    std::wifstream inFile(filePath);  /// 파일 리스트 불러오기
    if (inFile.is_open()) {
        std::wstring line;
        while (std::getline(inFile, line)) {
            fileList.push_back(line);  /// 파일 리스트에 추가
        }
        inFile.close();
        UpdateFileListUI(); 
        
    }
    else {
        MessageBox(hWnd, L"파일을 열 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);  // 파일 열기 오류 처리
    }
}


