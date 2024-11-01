#include "DW_SideMenu.h"

/// 네임 바 정적 메서드
LRESULT CALLBACK DrowWindow::WndProcSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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
        return pThis->handleMessageSB(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/// 사이드 메뉴의 이미지 버튼 생성
/// 현재 사이드 버튼 RECT크기 대비 이미지 작은 건 수정 예정
MakeButton sideNew(5, 5, 55, 55);
MakeButton sideSave(5, 65, 55, 115);
MakeButton sideLoad(5, 125, 55, 175);
MakeButton sideFM(5, 185, 55, 235);
MakeButton sideCredit(5, 245, 55, 295);

std::vector<PINFO>* penMemory = new std::vector<PINFO>; /// 추가

RECT mouseSide;     /// InterSect를 위한 마우스 좌표 받을 RECT
RECT aSide;         /// InterSect로 반환되는 RECT

HWND DW_SideMenu::hListBox = nullptr; // 초기값 설정

// 파일 이름을 저장할 벡터 추가
std::vector<std::wstring> fileList; // 파일 목록 벡터

std::wstring filePath;
std::wstring DW_SideMenu::filePath; // 정의

// 파일 매니저 버튼 클릭 여부를 체크하는 변수
bool isListBoxVisible = true; // 초기값: 리스트박스가 보이도록 설정

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


/// 네임 바 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
    {
        // 리스트박스 생성 예시
        DW_SideMenu::hListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
            0, 0, 100, 100, hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);

        // 초기 상태에서 리스트박스를 숨김
        ShowWindow(DW_SideMenu::hListBox, SW_HIDE);

        // 파일 목록을 리스트박스에 추가
        populateFileList(DW_SideMenu::hListBox); // .txt 파일 목록을 리스트박스에 추가합니다.

        break;
    }
    case WM_COMMAND:
    {
        // 리스트박스 더블 클릭 메시지 처리
        if (LOWORD(wParam) == 101 && HIWORD(wParam) == LBN_DBLCLK) {
            int selectedIndex = SendMessage(DW_SideMenu::hListBox, LB_GETCURSEL, 0, 0);
            if (selectedIndex != LB_ERR) {
                // 선택한 항목의 텍스트 가져오기
                wchar_t selectedFileName[256] = {};
                SendMessage(DW_SideMenu::hListBox, LB_GETTEXT, selectedIndex, (LPARAM)selectedFileName);

                // 선택한 파일 이름
                std::wstring selectedFile(selectedFileName);

                // 파일 경로 가져오기
                std::wstring desktopPath = getFilePath();

                // 전체 파일 경로 생성
                DW_SideMenu::filePath = desktopPath + selectedFile;

                // 파일 존재 여부 확인
                DWORD fileAttr = GetFileAttributesW(DW_SideMenu::filePath.c_str());
                if (fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
                    FileManager::fileManager.selectFileMode(SD_FILEMANAGER_BT, hWnd, penMemory); /// 추가
                }
                else {
                    // 파일이 존재하지 않음
                    MessageBox(hWnd, L"파일이 존재하지 않습니다.", L"알림", MB_OK);

                }
            }
            break;
        }
    case WM_LBUTTONDOWN:
    {
        /// 좌클릭시 현재 마우스 좌표(lParam)을 받아 마우스 RECT 생성
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        mouseSide.left = x;
        mouseSide.top = y;
        mouseSide.right = mouseSide.left + 1;
        mouseSide.bottom = mouseSide.top + 1;

        /// 파일 뉴파일
        if (IntersectRect(&aSide, &mouseSide, &sideNew.rectButton)) {
            /// 메세지 박스 없애고 if문(InTerSect)내부에 실행 코드 추가
            /// 메세지 박스는 이미지 버튼 활성화 여부 알아보기 위해 임시로 추가한 것
            if (function->getDrawLInfoEmpty()) { break; }
            if (!function->getIsReplay()) {
                SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_CLEAR_BT, 0);
                SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)L"이름 없음"); /// 2024_GuestBook_Team1로 메시지 전달 /// 추가
            }
        }

        /// 파일 세이브
        else if (IntersectRect(&aSide, &mouseSide, &sideSave.rectButton)) {
            /// 메세지 박스 없애고 if문(InTerSect)내부에 실행 코드 추가
            FileManager::fileManager.selectFileMode(SAVE, hWnd, penMemory); /// 추가
        }

        /// 파일 로드
        else if (IntersectRect(&aSide, &mouseSide, &sideLoad.rectButton)) {
            /// 메세지 박스 없애고 if문(InTerSect)내부에 실행 코드 추가
            FileManager::fileManager.selectFileMode(LOAD, hWnd, penMemory); /// 추가
            SendMessage(WndFunc::toolWnd, WM_COMMAND, TL_PLAY_BT, 0); /// 추가
        }

        /// 파일 매니저
        else if (IntersectRect(&aSide, &mouseSide, &sideFM.rectButton)) {
            // 리스트박스 보이기/숨기기 처리
            isListBoxVisible = !isListBoxVisible; // 현재 상태 반전

            ShowWindow(DW_SideMenu::hListBox, isListBoxVisible ? SW_HIDE : SW_SHOW); // 리스트박스 보이기/숨기기
        }

        /// 크레딧
        else if (IntersectRect(&aSide, &mouseSide, &sideCredit.rectButton)) {
            /// 메세지 박스 없애고 if문(InTerSect)내부에 실행 코드 추가
            MessageBox(hWnd, L"Credit", L"Credit", MB_OK);
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        /// 이미지 버튼 이미지 등록 및 그리기 실행 (필요시 MakeButton.h(.cpp) 참고)
        sideNew.drawRectButton(hdc, IDI_NEW_FILE_ICON);
        sideSave.drawRectButton(hdc, IDI_SAVE_ICON);
        sideLoad.drawRectButton(hdc, IDI_LOAD_ICON);
        sideFM.drawRectButton(hdc, IDI_FILEMANAGER_ICON);
        sideCredit.drawRectButton(hdc, IDI_CREDIT_ICON);

        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
    }
}
