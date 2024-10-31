#include "DW_Canvas.h"

// 멤버 변수 추가
bool isDrawing = false;    // 드로잉 상태 플래그
POINT lastPoint;           // 이전 점의 좌표

/// 네임 바 정적 메서드
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
        return pThis->handleMessageCV(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/// 네임 바 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        // 드로잉 시작 설정
        isDrawing = true;
        lastPoint.x = LOWORD(lParam);
        lastPoint.y = HIWORD(lParam);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (isDrawing) {
            // 마우스가 눌린 상태로 이동하면 선을 그립니다
            HDC hdc = GetDC(hWnd);

            MoveToEx(hdc, lastPoint.x, lastPoint.y, NULL);  // 이전 점으로 이동
            LineTo(hdc, LOWORD(lParam), HIWORD(lParam));    // 현재 위치까지 선을 그림

            lastPoint.x = LOWORD(lParam);  // 현재 위치를 이전 위치로 업데이트
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
