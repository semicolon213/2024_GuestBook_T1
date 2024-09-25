#include "Window.h"
#include "PenThickness.h"

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
    static BlueWindow* pBW = nullptr;
    static HWND d_hWnd = nullptr;
    static HWND b_hWnd = nullptr;

    switch (message)
    {
    case WM_CREATE :
        function = make_unique<Function>();
        fileManager = make_unique<FileManager>(hWnd);
        colorPalette = make_unique<ColorPalette>();

        fileManager->getInstance().InitializePanels(hWnd);  /*�г� �ʱ�ȭ */

        GetClientRect(hWnd, &MainRT);

        CreateWindowW(L"STATIC", L"�̸� ����", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 10, 10, 100, 30, hWnd, (HMENU)"FILE_NAME", hInst, nullptr);
        SideMenu = CreateWindowW(L"BUTTON", L":", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, MainRT.right - 49, 10, 30, 30, hWnd, (HMENU)"SIDE_MENU", hInst, nullptr);
        CreateWindowW(L"BUTTON", L"��1", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 5, 55, 30, 30, hWnd, (HMENU)COLOR1, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"��2", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 40, 55, 30, 30, hWnd, (HMENU)COLOR2, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"��3", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 75, 55, 30, 30, hWnd, (HMENU)COLOR3, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"����", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 110, 55, 30, 30, hWnd, (HMENU)P_WIDTH, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"CLEAR", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 145, 55, 60, 30, hWnd, (HMENU)CLEAR, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"NEW", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 210, 55, 55, 30, hWnd, (HMENU)NEW_FILE, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"SAVE", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 280, 55, 60, 30, hWnd, (HMENU)ID_SAVE_BUTTON, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"LOAD", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 350, 55, 60, 30, hWnd, (HMENU)LOAD, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"MANAGER", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 420, 55, 60, 30, hWnd, (HMENU)FILE_MANAER, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"CREDIT", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 490, 55, 60, 30, hWnd, (HMENU)CREDIT, hInst, nullptr);
        ///������ �ְ��� ���� �ϼ� �ؿ� �ڵ�
        CreateWindowW(L"BUTTON", L"STOP", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 630, 55, 60, 30, hWnd, (HMENU)BUTTON_ID, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"PLAY", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 560, 55, 60, 30, hWnd, (HMENU)PLAY, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"STOP", WS_CHILD | WS_VISIBLE /*| BS_OWNERDRAW*/, 630, 55, 60, 30, hWnd, (HMENU)STOP, hInst, nullptr);
        
        CreateWindowW(L"BUTTON", L"�⺻", WS_CHILD | WS_VISIBLE, 700, 55, 40, 30, hWnd, (HMENU)BASIC, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"��", WS_CHILD | WS_VISIBLE, 750, 55, 30, 30, hWnd, (HMENU)BRUSH, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"����", WS_CHILD | WS_VISIBLE, 700, 55, 40, 30, hWnd, (HMENU)PENCIL, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"��������", WS_CHILD | WS_VISIBLE, 780, 55, 60, 30, hWnd, (HMENU)SPRAY, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"��Ŀ", WS_CHILD | WS_VISIBLE, 850, 55, 40, 30, hWnd, (HMENU)MARKER, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"������", WS_CHILD | WS_VISIBLE, 900, 55, 50, 30, hWnd, (HMENU)PEN, hInst, nullptr);
        CreateWindowW(L"BUTTON", L"�簢��", WS_CHILD | WS_VISIBLE, 950, 55, 40, 30, hWnd, (HMENU)RECTANGLE, hInst, nullptr);


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
        /// �� ���� ����
        case P_WIDTH: {
            PenThickness penThickness;
            penThickness.Show(hInst, hWnd);
            break;
        }
        case COLOR1:
            if (penNum == 0)
                colorPalette->colorSelect(hWnd, penNum);
            else penNum = 0;
            break;

        case COLOR2:
            if (penNum == 1)
                colorPalette->colorSelect(hWnd, penNum);
            else penNum = 1;
            break;

        case COLOR3:
            if (penNum == 2)
                colorPalette->colorSelect(hWnd, penNum);
            else penNum = 2;
            break;

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

        case ID_SAVE_BUTTON:
            fileManager->SaveFile();
            break;

        case ID_FILE_LIST:
            /* ���� ����Ʈ �ڽ����� ���õ� ������ ó���ϴ� �ڵ带 �־����*/

        case PLAY:
            function->replayThread(hWnd);
            break;

        case STOP:
            function->setIsReplay(false);
            function->setIsTerminate(true);
            break;

        // ��ư ��� ���ظ��ؼ� ���� �ȵǴ���
        case BASIC:
            function->setBShape(BASIC);
            break;

        case BRUSH:
            function->setBShape(BRUSH);
            break;

        case PENCIL:
            function->setBShape(PENCIL);
            break;

        case SPRAY:
            function->setBShape(SPRAY);
            break;

        case MARKER:
            function->setBShape(MARKER);
            break;

        case PEN:
            function->setBShape(PEN);
            break;

        case RECTANGLE:
            function->setBShape(RECTANGLE);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);            
        }
        break;
    }


    
    case WM_MOUSEMOVE:
        if (function->getIsReplay()) break;
        drawPInfo.lParam = lParam;
        drawPInfo.pColor = colorPalette->getColor(penNum);
        drawPInfo.pTime = (DWORD)GetTickCount64();
        drawPInfo.pWidth = PenThickness::GetPenWidth(); /// �� ���� ����
        drawPInfo.state = message;
        function->draw(hWnd, drawPInfo, true); // �귯�� ��� �߰��Ϸ��� �ش� RECTANGLE �� �˸´� ������ ������ ��.
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        if (function->getIsReplay()) break;
        drawPInfo.lParam = lParam;
        drawPInfo.pColor = colorPalette->getColor(penNum);
        drawPInfo.pTime = (DWORD)GetTickCount64();
        drawPInfo.pWidth = PenThickness::GetPenWidth(); /// �� ���� ����
        drawPInfo.state = message;
        function->mouseUD(drawPInfo,true);

        break;

    case WM_RBUTTONDOWN :
        
        break;

    case WM_SIZE:
        fileManager->getInstance().ResizePanels(hWnd, lParam);  /*�г� ũ�� ���� �Լ� ȣ��*/
        GetClientRect(hWnd, &MainRT);
        MoveWindow(d_hWnd, 0, 0, MainRT.right, MainRT.bottom, TRUE);          //...������ ����
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hbr = (HBRUSH)SelectObject(hdc,CreateSolidBrush(RGB(249,249,249)));
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        GetClientRect(hWnd, &MainRT);
        
        Rectangle(hdc, -1, 52, MainRT.right + 1, 99);       //�޴��� �����               ũ�� ���� �ȵ����� �ϴ� ����;;
        hbr = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
        //����� ����� (ũ�� 1300X750)
        Rectangle(hdc, (MainRT.right - 1300) / 2, (MainRT.bottom - 750) / 2 + 100, (MainRT.right + 1300) / 2, (MainRT.bottom + 750) / 2);

        if (!function->getIsReplay())
        {
            for (const auto& record : function->getDrawLInfo().pInfo)
            {
                function->setBShape(record.bShape);

                switch (record.state)
                {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    function->mouseUD(record, false);
                    break;

                case WM_MOUSEMOVE:
                    function->draw(hWnd, record, false);
                    break;

                default:
                    break;
                }
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
