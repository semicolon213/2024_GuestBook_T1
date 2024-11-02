#include "DW_FileManager.h"

HWND DW_FileManager::hListBox = nullptr; // 초기값 설정

// 파일 이름을 저장할 벡터 추가
std::vector<std::wstring> fileList; // 파일 목록 벡터

std::wstring filePath;
std::wstring DW_FileManager::filePath; // 정의

// 파일 매니저 버튼 클릭 여부를 체크하는 변수

std::wstring getFilePath() {
    return L"C:\\2024_GuestBook_T1\\file\\"; // 절대 경로로 변경
}

void populateFileList(HWND hListBox) {
    std::wstring filePath = getFilePath();
    std::wcout << L"Searching in: " << filePath << std::endl; // 추가된 로그
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW((filePath + L"*.txt").c_str(), &findFileData); // .txt 파일만 찾기

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // 파일 이름을 리스트박스에 추가
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { // 디렉토리 제외
                SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)findFileData.cFileName);
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

/// 네임 바 정적 메서드
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
        return pThis->handleMessageFM(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}



/// 네임 바 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageFM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        // 리스트박스 생성 예시
        DW_FileManager::hListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
            0, 0, 500, 500, hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);

        // 초기 상태에서 리스트박스를 숨김
        ShowWindow(DW_FileManager::hListBox, SW_HIDE);

        // 파일 목록을 리스트박스에 추가
        populateFileList(DW_FileManager::hListBox); // .txt 파일 목록을 리스트박스에 추가합니다.
        break;
    }
    case WM_SIZE:
    {
        break;
    }
    case WM_LBUTTONDOWN:
    {
        //MessageBox(hWnd, L"dd", L"dd", MB_OK);
        break;
    }
    case WM_LBUTTONDBLCLK:
    {


        break;
    }
    case WM_COMMAND:
    {
            // 리스트박스 더블 클릭 메시지 처리
        if (LOWORD(wParam) == 101 && HIWORD(wParam) == LBN_DBLCLK) {
            int selectedIndex = SendMessage(DW_FileManager::hListBox, LB_GETCURSEL, 0, 0);
            if (selectedIndex != LB_ERR) {
                // 선택한 항목의 텍스트 가져오기
                wchar_t selectedFileName[256] = {};
                SendMessage(DW_FileManager::hListBox, LB_GETTEXT, selectedIndex, (LPARAM)selectedFileName);

                // 선택한 파일 이름
                std::wstring selectedFile(selectedFileName);

                // 파일 경로 가져오기
                std::wstring desktopPath = getFilePath();

                // 전체 파일 경로 생성
                DW_FileManager::filePath = desktopPath + selectedFile;

                // 파일 존재 여부 확인
                DWORD fileAttr = GetFileAttributesW(DW_FileManager::filePath.c_str());
                if (fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
                    FileManager::fileManager.selectFileMode(SD_FILEMANAGER_BT, hWnd, DW_SideMenu::penMemory);
                }
                else {
                    // 파일이 존재하지 않음
                    MessageBox(hWnd, L"파일이 존재하지 않습니다.", L"알림", MB_OK);

                }
            }

        }
        break;
    }
    case WM_PAINT:
    {
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

    

