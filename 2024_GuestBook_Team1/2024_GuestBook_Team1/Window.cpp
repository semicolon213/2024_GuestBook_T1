#include "Window.h"
#include "FileManager.h"

// 멤버 변수 초기화
unique_ptr<Window> Window::sinTonIns = nullptr;
once_flag Window::flag;
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM Window::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = StaticWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YUHAN256));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(243,243,243));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_YUHAN));

    WNDCLASSEXW nbcex = wcex;


    return RegisterClassExW(&wcex);
}


static RECT DesktopRT;      //사용자 화면 크기 받기용
static RECT MainRT;         //메인 윈도우 크기 받기용

//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//


BOOL Window::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    GetWindowRect(GetDesktopWindow(), &DesktopRT);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        (DesktopRT.right - 1600) / 2, (DesktopRT.bottom - 900) / 2, 1600, 900, nullptr, nullptr, hInstance, nullptr);

    //제목 표시줄 색상 변경
    int aElements[] = { COLOR_ACTIVECAPTION };
    DWORD aOldColors[1];
    DWORD aNewColors[1];
    aOldColors[0] = GetSysColor(aElements[0]);
    aNewColors[0] = RGB(243, 243, 243);
    SetSysColors(1, aElements, aNewColors);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT Window::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return sinTonIns->WndProc(hWnd, message, wParam, lParam);
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//



LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    string FileName = "제목 없음";
    HWND SideMenu = NULL;

    switch (message)
    {
    case WM_CREATE :
        FileManager::InitializePanels(hWnd);  /*패널 초기화 */
        function = make_unique<GB_Function>();

        GetClientRect(hWnd, &MainRT);

        CreateWindowW(L"STATIC", L"이름 없음", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 10, 10, 100, 30, hWnd, (HMENU)"FILE_NAME", hInst, nullptr);
        SideMenu = CreateWindowW(L"BUTTON", L":", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, MainRT.right - 49, 10, 30, 30, hWnd, (HMENU)"SIDE_MENU", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"색1", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 5, 55, 30, 30, hWnd, (HMENU)"COLOR1", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"색2", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 40, 55, 30, 30, hWnd, (HMENU)"COLOR2", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"색3", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 75, 55, 30, 30, hWnd, (HMENU)"COLOR3", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"굵기", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 110, 55, 30, 30, hWnd, (HMENU)"P_WIDTH", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"CLEAR", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 145, 55, 60, 30, hWnd, (HMENU)"CLEAR", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"NEW", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 210, 55, 55, 30, hWnd, (HMENU)"NEW_FILE", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"SAVE", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 280, 55, 60, 30, hWnd, (HMENU)"SAVE", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"LOAD", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 350, 55, 60, 30, hWnd, (HMENU)"LOAD", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"MANAGER", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 420, 55, 60, 30, hWnd, (HMENU)"FILE_MANAER", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"CREDIT", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 490, 55, 60, 30, hWnd, (HMENU)"CREDIT", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"PLAY", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 560, 55, 60, 30, hWnd, (HMENU)"PLAY", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"STOP", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 630, 55, 60, 30, hWnd, (HMENU)"STOP", hInst, nullptr);

        break;


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_FILE_LIST:
            /* 파일 리스트 박스에서 선택된 파일을 처리하는 코드를 넣어야함*/
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_LBUTTONDOWN:
        function->mouseUD(lParam, (DWORD)GetTickCount64(), message, 10, RGB(255, 0, 0));
        break;

    case WM_MOUSEMOVE:
        function->draw(hWnd, lParam, (DWORD)GetTickCount64(), message, 10, RGB(255, 0, 0));
        break;

    case WM_LBUTTONUP:
        function->mouseUD(lParam, (DWORD)GetTickCount64(), message, 10, RGB(255, 0, 0));

        break;

    case WM_RBUTTONDOWN :

        break;
    case WM_SIZE:
        FileManager::ResizePanels(hWnd, lParam);  /*패널 크기 조정 함수 호출*/
        MoveWindow(SideMenu, MainRT.right - 49, 10, 30, 30, TRUE);          //...다음에 구현
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hbr = (HBRUSH)SelectObject(hdc,CreateSolidBrush(RGB(249,249,249)));
        HPEN hPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        GetClientRect(hWnd, &MainRT);
        
        Rectangle(hdc, -1, 52, MainRT.right + 1, 99);       //메뉴바 만들기               크흑 맘에 안들지만 일단 생성;;
        hbr = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
        //서명란 만들기 (크기 1200X700)
        Rectangle(hdc, (MainRT.right - 1200) / 2, (MainRT.bottom - 700) / 2 + 100, (MainRT.right + 1200) / 2, (MainRT.bottom + 700) / 2);


        bool LBState = false;
        int x, y;
        for (const auto& record : function->drawLInfo.pInfo)
        {
            x = LOWORD(record.lParam);
            y = HIWORD(record.lParam);

            switch (record.state)
            {
            case WM_LBUTTONDOWN:
                LBState = true;
                MoveToEx(hdc, x, y, NULL);
                LineTo(hdc, x, y);
                break;

            case WM_MOUSEMOVE:
                LineTo(hdc, x, y);
                break;
            case WM_LBUTTONUP:
                LBState = false;
                break;
            default:
                break;
            }
        }

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    }
    return (INT_PTR)FALSE;
}

Window* Window::GetInstance()
{
    call_once(flag, []          ///익명함수
        {
            sinTonIns.reset(new Window);
        });
    return sinTonIns.get();
}