#include "Window.h"

using namespace std;

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
    static HWND d_hWnd = nullptr;
    static HWND b_hWnd = nullptr;

    switch (message)
    {
    case WM_CREATE :

        GetClientRect(hWnd, &MainRT);


        DrowBT = CreateWindowW(L"BUTTON", L"�����ϱ�", WS_CHILD | WS_VISIBLE,
            (MainRT.right / 2) - 120, (MainRT.bottom / 2) - 170, 240, 100, hWnd, (HMENU)DEF_DROW_BT, hInst, nullptr);
        LoadBT = CreateWindowW(L"BUTTON", L"�ҷ�����", WS_CHILD | WS_VISIBLE,
            (MainRT.right / 2) - 120, (MainRT.bottom / 2) - 50, 240, 100, hWnd, (HMENU)DEF_LOAD_BT, hInst, nullptr);
        CreditBT = CreateWindowW(L"BUTTON", L"CREDIT", WS_CHILD | WS_VISIBLE,
            (MainRT.right / 2) - 120, (MainRT.bottom / 2) + 70, 240, 100, hWnd, (HMENU)DEF_CREDIT_BT, hInst, nullptr);

        dWindow = new DrowWindow(hInst);
        dWindow->Create(hWnd, 0, 0, MainRT.right, MainRT.bottom);
        dWindow->Show(FALSE);
        d_hWnd = dWindow->GetHWND();

        break;

    case WM_COMMAND:
    {

        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case DEF_DROW_BT:
            EnableWindow(d_hWnd, true);
            dWindow->ToolCnt = true;
            dWindow->Show(true);
            EnableWindow(DrowBT, FALSE);
            EnableWindow(LoadBT, FALSE);
            EnableWindow(CreditBT, FALSE);

            break;

        case DEF_LOAD_BT:
            EnableWindow(d_hWnd, true);
            dWindow->ToolCnt = FALSE;
            dWindow->Show(true);
            EnableWindow(DrowBT, FALSE);
            EnableWindow(LoadBT, FALSE);
            EnableWindow(CreditBT, FALSE);

            /*
            
            ���⿡�� ���� �ҷ����±�� �߰����ּ���
            
            */

            break;


        case DEF_CREDIT_BT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);            
        }
        break;
    }

    case WM_SIZE:
        //fileManager->getInstance().ResizePanels(hWnd, lParam);  /*�г� ũ�� ���� �Լ� ȣ��*/
        GetClientRect(hWnd, &MainRT);

        MoveWindow(DrowBT, (MainRT.right / 2) - 120, (MainRT.bottom / 2) - 170, 240, 100, TRUE);
        MoveWindow(LoadBT, (MainRT.right / 2) - 120, (MainRT.bottom / 2) - 50, 240, 100, TRUE);
        MoveWindow(CreditBT, (MainRT.right / 2) - 120, (MainRT.bottom / 2) + 70, 240, 100, TRUE);

        MoveWindow(d_hWnd, 0, 0, MainRT.right, MainRT.bottom, TRUE);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hbr = (HBRUSH)SelectObject(hdc,CreateSolidBrush(RGB(249,249,249)));
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        GetClientRect(hWnd, &MainRT);

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
