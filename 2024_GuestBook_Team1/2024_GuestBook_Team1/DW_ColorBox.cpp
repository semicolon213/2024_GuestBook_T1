#include "DW_ColorBox.h"
#include <Windows.h>



/// ���� �� ���� �޼���
LRESULT CALLBACK DrowWindow::WndProcCP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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
        return pThis->handleMessageCP(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}



/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageCP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    
    switch (message)
    {
    case WM_CREATE:
    {
        break;
    }
    case WM_LBUTTONDOWN:
    {
                  
        break;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDC_CLOSE_BUTTON) {
            ShowWindow(hWnd, SW_HIDE);
            return 0;
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

      

        EndPaint(hWnd, &ps);
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



