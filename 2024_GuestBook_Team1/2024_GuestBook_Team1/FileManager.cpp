/**
@author 김수길
@date 2024.09.01
    파일매니저 기능 구현을 위한 임시 저장버튼 생성 
    파일리스트 구현 (좌표값 배치 및 ui는 기능 구현 완료 후 수정작업 예정)
@todo 저장버튼 누르면 파일리스트에 들어가기
      좌표 재배치 및 UI 업데이트 
      다이얼로그에 저장하기 불러오기 구현 
**/



#include "FileManager.h"
#include "window.h"

std::vector<std::wstring> fileList;




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
void FileManager::Button(HWND hWnd) /*파일매니저 저장기능을 위한 임시 버튼 */
{
    getInstance().File_Manager_Button = CreateWindowW(L"Button", L"저장", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150,150,150,150, hWnd, (HMENU)ID_BUTTON_SAVE, GetModuleHandle(NULL), NULL);

    if (!getInstance().File_Manager_Button) /*버튼 생성에 자꾸 오류가 떠서 확인용.*/
    {
        MessageBox(hWnd, L"버튼 생성 실패", L"오류", MB_OK | MB_ICONERROR);
    }
}

/*void SaveFile(const std::wstring& fileName) 
{
    // 파일 저장 로직 넣기
    

    AddFileToList(fileName); 
} 아직 기능 구현 중 */


