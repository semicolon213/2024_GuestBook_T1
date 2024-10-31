#include "DW_Canvas.h"

// ��� ���� �߰�
bool isDrawing = false;    // ����� ���� �÷���
POINT lastPoint;           // ���� ���� ��ǥ

/// ���� �� ���� �޼���
LRESULT CALLBACK DrowWindow::WndProcCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
        return pThis->handleMessageCV(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        // ����� ���� ����
        isDrawing = true;
        lastPoint.x = LOWORD(lParam);
        lastPoint.y = HIWORD(lParam);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (isDrawing) {
            // ���콺�� ���� ���·� �̵��ϸ� ���� �׸��ϴ�
            HDC hdc = GetDC(hWnd);

            MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);  // ���� ������ �̵�
            LineTo(hdc, LOWORD(lParam), HIWORD(lParam));    // ���� ��ġ���� ���� �׸�

            lastPoint.x = LOWORD(lParam);  // ���� ��ġ�� ���� ��ġ�� ������Ʈ
            lastPoint.y = HIWORD(lParam);

            ReleaseDC(hWnd, hdc);
        }
        return 0;
    }
    case WM_LBUTTONUP:
    {
        isDrawing = false;
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);


        EndPaint(hWnd, &ps);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
