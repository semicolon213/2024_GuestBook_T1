#include "DW_ColorBox.h"
#include <Windows.h>

int DW_ColorBox::colorSelect = 0;

COLORREF DW_ColorBox::colorP[3] = { RGB(0,0,0),RGB(0,0,0),RGB(0,0,0) };

COLORREF DW_ColorBox::getColorNum() {
    return DW_ColorBox::colorP[DW_ColorBox::colorSelect];
}

void DW_ColorBox::setColorNum(int num, COLORREF color) {
    DW_ColorBox::colorP[num] = color;
}


/// 컬러팔레트 정적 메서드
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
        return pThis->handleMessageCP(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

RECT redB = { 5, 5, 40, 40 };
RECT blueB = { 45, 5, 80, 40 };
RECT aaa;
RECT m;

DW_ColorBox test;

/// 컬러팔레트 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageCP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    
    switch (message)
    {
    case WM_CREATE:
    {
        break;
    }
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        
        m.left = x;
        m.top = y;
        m.right = m.left + 1;
        m.bottom = m.top + 1;

        if(IntersectRect(&aaa, &redB, &m)) {
            MessageBox(hWnd, L"빨간버튼", L"빨간 버튼", MB_OK);
            DW_ColorBox::setColorNum(0, RGB(255, 0, 0));
        }

        if (IntersectRect(&aaa, &blueB, &m)) {
            MessageBox(hWnd, L"파란 버튼", L"파란 버튼", MB_OK);
            DW_ColorBox::setColorNum(0, RGB(0, 0, 255));
        }


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

        Rectangle(hdc, redB.left, redB.top, redB.right, redB.bottom);
        Rectangle(hdc, blueB.left, blueB.top, blueB.right, blueB.bottom);

        EndPaint(hWnd, &ps);
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



