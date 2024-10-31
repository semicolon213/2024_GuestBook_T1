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


/// 네임 바 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
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
            MessageBox(hWnd, L"new file", L"new file", MB_OK);
        }

        /// 파일 세이브
        else if (IntersectRect(&aSide, &mouseSide, &sideSave.rectButton)) {
            /// 메세지 박스 없애고 if문(InTerSect)내부에 실행 코드 추가
            FileManager::fileManager.selectFileMode(SAVE, hWnd, penMemory); /// 추가
        }

        /// 파일 로드
        else if (IntersectRect(&aSide, &mouseSide, &sideLoad.rectButton)) {
            /// 메세지 박스 없애고 if문(InTerSect)내부에 실행 코드 추가
            MessageBox(hWnd, L"file load", L"file load", MB_OK);
            FileManager::fileManager.selectFileMode(LOAD, hWnd, penMemory); /// 추가
            SendMessage(WndFunc::toolWnd, WM_COMMAND, TL_PLAY_BT, 0); /// 추가
        }

        /// 파일 매니저
        else if (IntersectRect(&aSide, &mouseSide, &sideFM.rectButton)) {
            /// 메세지 박스 없애고 if문(InTerSect)내부에 실행 코드 추가
            MessageBox(hWnd, L"file Manager", L"file Manager", MB_OK);
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





