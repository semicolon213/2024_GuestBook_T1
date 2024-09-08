#include "colorpalette.h" /// colorpalette.h ��� ������ �����Ѵ�

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
    static COLORREF acrCustClr[16]; /// ����� ���� ���� ���� ����
    static CHOOSECOLOR cc; /// ���� ���� â�� ����, ȣ���ϴ� ����ü
    static HBRUSH hBrush = NULL; /// ������ �����ϴ� �귯�� => �ʱⰪ NULL

    switch (uMsg) {
    case WM_LBUTTONDBLCLK: /// ���콺�� ����Ŭ�� �� �� ����
        ZeroMemory(&cc, sizeof(cc)); /// ���� ���� â ����, ȣ���� �⺻������ ����
        cc.lStructSize = sizeof(cc); /// ����ü�� ũ�� ����
        cc.hwndOwner = hwnd; /// ������ �ڵ� ����
        cc.lpCustColors = (LPDWORD)acrCustClr; /// lpCustColors : ����� ���� ���� �迭�� �ּ�
        cc.rgbResult = RGB(0, 0, 0); /// ���� â�� ó�� ���� �� �������� ���� (�ʱ� ���� : ������)
        cc.Flags = CC_FULLOPEN | CC_RGBINIT; /// ��ȭ ���� �÷��� ����

        if (ChooseColor(&cc)) { /// ���� ���� ��ȭ ���� ǥ��
            if (hBrush) {
                DeleteObject(hBrush); /// ���� �귯�� ����
            }
            hBrush = CreateSolidBrush(cc.rgbResult); /// ���ο� �귯�� ����
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
            DeleteObject(hBrush); /// �귯�� ����
        }
        PostQuitMessage(0); /// ���α׷� ����
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
    wc.lpszClassName = CLASS_NAME; /// Ŭ���� �̸� ����
    wc.style = CS_DBLCLKS; /// ����Ŭ�� �� �޽���

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"���� Ŭ���Ͽ� �� ����",
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
