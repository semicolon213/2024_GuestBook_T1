#include "DW_FileManager.h"

HWND DW_FileManager::hListBox2 = nullptr;

/// 네임 바 정적 메서드
LRESULT CALLBACK DrowWindow::WndProcFM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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
        return pThis->handleMessageFM(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}



/// 네임 바 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageFM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        DW_FileManager::hListBox2 = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
            0, 0, 100, 100, WndFunc::fileManager, (HMENU)102, GetModuleHandle(NULL), NULL);

        break;
    }
    case WM_SIZE:
    {
        break;
    }
    case WM_LBUTTONDOWN:
    {
        //MessageBox(hWnd, L"dd", L"dd", MB_OK);
        break;
    }
    case WM_LBUTTONDBLCLK:
    {
        // 마우스 왼쪽 버튼 더블 클릭
        MessageBox(hWnd, L"Left Button Double Clicked", L"Double Click", MB_OK);

        break;
    }
    case WM_COMMAND:
    {
        int wmId = wParam;

        switch (wmId)
        {

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

    

