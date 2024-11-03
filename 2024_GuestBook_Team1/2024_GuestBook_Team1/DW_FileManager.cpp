#include "DW_FileManager.h"

HWND DW_FileManager::hListBox = nullptr; // 초기값 설정

// 파일 이름을 저장할 벡터 추가
std::vector<std::wstring> fileList; // 파일 목록 벡터

std::wstring filePath;
std::wstring DW_FileManager::filePath; // 정의

std::wstring getFilePath() {
    PWSTR desktopPath = NULL;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &desktopPath);

    if (SUCCEEDED(result)) {
        std::wstring path(desktopPath);
        CoTaskMemFree(desktopPath);  // 메모리 해제
        return path + L"\\file\\";
    }
    else {
        return L"";  // 경로를 가져오지 못했을 경우 빈 문자열 반환
    }
}

void DW_FileManager::saveFileList(const std::vector<std::wstring>& fileList) {
    std::wstring filePath = getFilePath() + L"\\file\\FileList.txt";  // 바탕화면 경로에 저장
    std::wofstream outFile(filePath);
    if (outFile.is_open()) {
        for (const auto& fileName : fileList) {
            outFile << fileName << std::endl;
        }
        outFile.close();
    }
}

std::vector<std::wstring> DW_FileManager::loadFileList() {
    std::vector<std::wstring> fileList;
    std::wstring filePath = getFilePath() + L"\\file\\FileList.txt";  // 바탕화면 경로에서 로드
    std::wifstream inFile(filePath);
    std::wstring line;

    while (std::getline(inFile, line)) {
        fileList.push_back(line);
    }
    return fileList;
}

void populateFileList(HWND hListBox) {
    // 리스트박스를 초기화하지 않음으로써 기존 항목을 유지
    std::vector<std::wstring> savedFileList = DW_FileManager::loadFileList();

    // 기존 파일 목록을 리스트박스에 추가
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

    for (const auto& fileName : savedFileList) {
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)fileName.c_str());
    }

    // 새로운 파일을 검색 및 추가
    std::wstring filePath = getFilePath();
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW((filePath + L"*.txt").c_str(), &findFileData); // .txt 파일만 찾기

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // 파일이 목록에 이미 있는지 확인
                auto it = std::find(savedFileList.begin(), savedFileList.end(), findFileData.cFileName);
                if (it == savedFileList.end()) {
                    // 중복이 아닌 경우에만 추가
                    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)findFileData.cFileName);
                    savedFileList.push_back(findFileData.cFileName);
                }
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    // 업데이트된 파일 목록 저장
    DW_FileManager::saveFileList(savedFileList);
}

LRESULT CALLBACK DrowWindow::WndProcFM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    DrowWindow* pThis = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<DrowWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<DrowWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->handleMessageFM(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT DrowWindow::handleMessageFM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        RECT rect;
        GetClientRect(WndFunc::drowWnd, &rect);

        int width = rect.right - rect.left- 1400;
        int height = rect.bottom - rect.top - 570;

        DW_FileManager::hListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER,
            rect.left + 8, rect.top + 8, width, height, hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);

        SendMessage(DW_FileManager::hListBox, LB_SETHORIZONTALEXTENT, width, 0);

        ShowWindow(DW_FileManager::hListBox, SW_HIDE);

        populateFileList(DW_FileManager::hListBox);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 101 && HIWORD(wParam) == LBN_DBLCLK) {
            int selectedIndex = SendMessage(DW_FileManager::hListBox, LB_GETCURSEL, 0, 0);
            if (selectedIndex != LB_ERR) {
                wchar_t selectedFileName[256] = {};
                SendMessage(DW_FileManager::hListBox, LB_GETTEXT, selectedIndex, (LPARAM)selectedFileName);

                std::wstring selectedFile(selectedFileName);

                std::wstring desktopPath = getFilePath();

                DW_FileManager::filePath = desktopPath + selectedFile;

                DWORD fileAttr = GetFileAttributesW(DW_FileManager::filePath.c_str());
                if (fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
                    FileManager::fileManager.selectFileMode(SD_FILEMANAGER_BT, hWnd, DW_SideMenu::penMemory);
                }
                else {
                    MessageBox(hWnd, L"파일이 존재하지 않습니다.", L"알림", MB_OK);
                }
            }
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}