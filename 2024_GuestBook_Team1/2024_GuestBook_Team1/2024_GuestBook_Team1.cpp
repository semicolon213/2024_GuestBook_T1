/**
@author 김형균
@date 2024.08.01
    기본 그리기 코드 추가
    그린 내용 유지 코드 추가
@todo 그린 내용 replay 구현
**/

#include "2024_GuestBook_Team1.h"
#include "Resource.h"


#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, Window::GetInstance()->szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2024GUESTBOOKTEAM1, Window::GetInstance()->szWindowClass, MAX_LOADSTRING);
    Window::GetInstance()->MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!Window::GetInstance()->InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2024GUESTBOOKTEAM1));

    MSG msg;



    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
