#include "Window.h"
#include "FileManager.h"

// ��� ���� �ʱ�ȭ
unique_ptr<Window> Window::sinTonIns = nullptr;
once_flag Window::flag;
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
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



static RECT DesktopRT;      //����� ȭ�� ũ�� �ޱ��
static RECT MainRT;         //���� ������ ũ�� �ޱ��

//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//


BOOL Window::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    GetWindowRect(GetDesktopWindow(), &DesktopRT);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        (DesktopRT.right - 1600) / 2, (DesktopRT.bottom - 900) / 2, 1600, 900, nullptr, nullptr, hInstance, nullptr);

    //���� ǥ���� ���� ����
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//



LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    string FileName = "���� ����";
    static HWND SideMenu = NULL;
    static DrowWindow* dWindow = nullptr;
    static BlueWindow* pBW = nullptr;
    static HWND d_hWnd = nullptr;
    static HWND b_hWnd = nullptr;


    switch (message)
    {
    case WM_CREATE :
        InitializePanels(hWnd);  /*�г� �ʱ�ȭ */
        function = make_unique<GB_Function>();

        GetClientRect(hWnd, &MainRT);

        CreateWindowW(L"STATIC", L"�̸� ����", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 10, 10, 100, 30, hWnd, (HMENU)"FILE_NAME", hInst, nullptr);
        SideMenu = CreateWindowW(L"BUTTON", L":", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, MainRT.right - 49, 10, 30, 30, hWnd, (HMENU)"SIDE_MENU", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"��1", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 5, 55, 30, 30, hWnd, (HMENU)"COLOR1", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"��2", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 40, 55, 30, 30, hWnd, (HMENU)"COLOR2", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"��3", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 75, 55, 30, 30, hWnd, (HMENU)"COLOR3", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"����", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 110, 55, 30, 30, hWnd, (HMENU)"P_WIDTH", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"CREDIT", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 145, 55, 60, 30, hWnd, (HMENU)"CREDIT", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"NEW", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 210, 55, 55, 30, hWnd, (HMENU)"NEW_FILE", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"SAVE", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 280, 55, 60, 30, hWnd, (HMENU)"SAVE", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"LOAD", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 350, 55, 60, 30, hWnd, (HMENU)"LOAD", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"MANAGER", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 420, 55, 60, 30, hWnd, (HMENU)"FILE_MANAER", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"CLEAR", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 490, 55, 60, 30, hWnd, (HMENU)CLEAR_BUTTON, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"PLAY", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 560, 55, 60, 30, hWnd, (HMENU)PLAY_BUTTON, hInst, nullptr);
        //CreateWindowW(L"BUTTON", L"STOP", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 630, 55, 60, 30, hWnd, (HMENU)BUTTON_ID, hInst, nullptr);

        dWindow = new DrowWindow(hInst);
        dWindow->Create(hWnd, 0, 0, MainRT.right, MainRT.bottom);
        dWindow->Show(FALSE);
        d_hWnd = dWindow->GetHWND();

        pBW = new BlueWindow(hInst);

        pBW->Create(hWnd, 0, 0, MainRT.right, MainRT.bottom);

        pBW->Show(FALSE);

        b_hWnd = pBW->GetHWND();
        break;


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case BUTTON_ID:
            dWindow->Show(true);
            EnableWindow(GetDlgItem(hWnd, BUTTON_ID), FALSE);
            break;

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_FILE_LIST:
            /* ���� ����Ʈ �ڽ����� ���õ� ������ ó���ϴ� �ڵ带 �־����*/
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_LBUTTONDOWN:
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
        ResizePanels(hWnd, lParam);  /*�г� ũ�� ���� �Լ� ȣ��*/
        GetClientRect(hWnd, &MainRT);
        MoveWindow(d_hWnd, 0, 0, MainRT.right, MainRT.bottom, TRUE);          //...������ ����
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hbr = (HBRUSH)SelectObject(hdc,CreateSolidBrush(RGB(249,249,249)));
        HPEN hPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        GetClientRect(hWnd, &MainRT);
        
        Rectangle(hdc, -1, 52, MainRT.right + 1, 99);       //�޴��� �����               ũ�� ���� �ȵ����� �ϴ� ����;;
        hbr = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
        //����� ����� (ũ�� 1300X750)
        Rectangle(hdc, (MainRT.right - 1300) / 2, (MainRT.bottom - 750) / 2 + 100, (MainRT.right + 1300) / 2, (MainRT.bottom + 750) / 2);


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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
    call_once(flag, []          ///�͸��Լ�
        {
            sinTonIns.reset(new Window);
        });
    return sinTonIns.get();
}