/**
@author 김수길
@date 2024.09.08
    파일리스트 구현 (좌표값 배치 및 ui는 기능 구현 완료 후 수정작업 예정)
    저장버튼 누르면 파일리스트에 생성
    실제 경로에 저장
@todo 불러오기 만들기
      좌표 재배치 및 UI 업데이트 
      다이얼로그에 저장하기 불러오기 구현 
**/



#include "FileManager.h"

std::vector<std::wstring> fileList;

FileManager::FileManager() : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), hWnd(nullptr) {
    
}

// HWND를 받는 생성자 정의
FileManager::FileManager(HWND hWnd) : hRightPanel(nullptr), hFileListBox(nullptr), hInst(nullptr), hWnd(hWnd) {
    
}


/*패널을 초기화*/ 
void FileManager::InitializePanels(HWND hWnd)
{
    static const int fixedWidth = 300; // 오른쪽 패널의 고정된 너비를 정의 

    /* 오른쪽 패널을 생성 */
    getInstance().hRightPanel = CreateWindowW(L"STATIC", L"File Manager", WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, fixedWidth, 600, hWnd, (HMENU)ID_RIGHT_PANEL, GetModuleHandle(NULL), NULL);

    /* 파일 리스트 박스 생성 */
    getInstance().hFileListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        10, 50, fixedWidth - 20, 750, getInstance().hRightPanel, (HMENU)ID_FILE_LIST, GetModuleHandle(NULL), NULL);

 
}

/* 패널 크기*/
void FileManager::ResizePanels(HWND hWnd, LPARAM lParam)
{
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    static const int fixedWidth = 300;

    /* 패널 크기 조정 */
    MoveWindow(getInstance().hRightPanel, width - fixedWidth, 0, fixedWidth, height, TRUE);
    
}void FileManager::AddFileToList(const std::wstring& fileName)
{
    fileList.push_back(fileName);
    UpdateFileListUI();
}

void FileManager::UpdateFileListUI()
{
    /*리스트 박스를 비움*/
    SendMessage(getInstance().hFileListBox, LB_RESETCONTENT, 0, 0);

    /* 파일 리스트를 리스트 박스에 추가*/
    for (const auto& file : fileList)
    {
        SendMessage(getInstance().hFileListBox, LB_ADDSTRING, 0, (LPARAM)file.c_str());
    }
}





void FileManager::SaveFile() {
    WCHAR file_name[256] = L"";
    OPENFILENAME OFN;
    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hWnd; 
    OFN.lpstrFilter = L"Guest Book(*.gb)\0*.gb";
    OFN.lpstrDefExt = L"gb";
    OFN.lpstrFile = file_name;
    OFN.nMaxFile = 256;
    OFN.lpstrInitialDir = L"C:\\Users\\Public\\Documents";
    OFN.Flags = OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&OFN)) {
        
        std::wofstream outFile(file_name);  /// 파일생성

        if (outFile.is_open()) {
            
            
            outFile.close();  // 파일 닫기

            AddFileToList(file_name);  /// 선택한 파일명을 리스트에 추가
        }
        else {
           
            MessageBox(hWnd, L"파일 저장에 실패!", L"오류", MB_OK | MB_ICONERROR);
        }
    }
    
}



