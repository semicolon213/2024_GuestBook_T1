#include "colorpalette.h" /// colorpalette.h 헤더 파일을 포함한다

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
    static COLORREF acrCustClr[16]; /// 사용자 정의 색상 저장 공간
    static CHOOSECOLOR cc; /// 색상 선택 창을 설정, 호출하는 구조체
    static HBRUSH hBrush = NULL; /// 색상을 저장하는 브러쉬 => 초기값 NULL

    switch (uMsg) {
    case WM_LBUTTONDBLCLK: /// 마우스가 더블클릭 될 때 실행
        ZeroMemory(&cc, sizeof(cc)); /// 색상 선택 창 설정, 호출을 기본값으로 설정
        cc.lStructSize = sizeof(cc); /// 구조체의 크기 설정
        cc.hwndOwner = hwnd; /// 소유자 핸들 설정
        cc.lpCustColors = (LPDWORD)acrCustClr; /// lpCustColors : 사용자 정의 색상 배열의 주소
        cc.rgbResult = RGB(0, 0, 0); /// 색상 창이 처음 열릴 떄 보여지는 색상 (초기 색상 : 검은색)
        cc.Flags = CC_FULLOPEN | CC_RGBINIT; /// 대화 상자 플래그 설정

        if (ChooseColor(&cc)) { /// 색상 선택 대화 상자 표시
            if (hBrush) {
                DeleteObject(hBrush); /// 기존 브러쉬 삭제
            }
            hBrush = CreateSolidBrush(cc.rgbResult); /// 새로운 브러쉬 생성
            InvalidateRect(hwnd, NULL, TRUE); 
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (hBrush) {
            FillRect(hdc, &ps.rcPaint, hBrush);
        }
        EndPaint(hwnd, &ps);
    } break;

    case WM_DESTROY:
        if (hBrush) {
            DeleteObject(hBrush); /// 브러쉬 삭제
        }
        PostQuitMessage(0); /// 프로그램 종료
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME; /// 클래스 이름 설정
    wc.style = CS_DBLCLKS; /// 더블클릭 시 메시지

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"더블 클릭하여 색 선택",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); 
        DispatchMessage(&msg);
    }

    return 0;
}
