#include "DW_SideMenu.h"

/// ���� �� ���� �޼���
LRESULT CALLBACK DrowWindow::WndProcSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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
        return pThis->handleMessageSB(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/// ���̵� �޴��� �̹��� ��ư ����
/// ���� ���̵� ��ư RECTũ�� ��� �̹��� ���� �� ���� ����
MakeButton sideNew(5, 5, 55, 55);
MakeButton sideSave(5, 65, 55, 115);
MakeButton sideLoad(5, 125, 55, 175);
MakeButton sideFM(5, 185, 55, 235);
MakeButton sideCredit(5, 245, 55, 295);

std::vector<PINFO>* penMemory = new std::vector<PINFO>; /// �߰�

RECT mouseSide;     /// InterSect�� ���� ���콺 ��ǥ ���� RECT
RECT aSide;         /// InterSect�� ��ȯ�Ǵ� RECT


/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        /// ��Ŭ���� ���� ���콺 ��ǥ(lParam)�� �޾� ���콺 RECT ����
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        mouseSide.left = x;
        mouseSide.top = y;
        mouseSide.right = mouseSide.left + 1;
        mouseSide.bottom = mouseSide.top + 1;

        /// ���� ������
        if (IntersectRect(&aSide, &mouseSide, &sideNew.rectButton)) {
            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            /// �޼��� �ڽ��� �̹��� ��ư Ȱ��ȭ ���� �˾ƺ��� ���� �ӽ÷� �߰��� ��
            MessageBox(hWnd, L"new file", L"new file", MB_OK);
        }

        /// ���� ���̺�
        else if (IntersectRect(&aSide, &mouseSide, &sideSave.rectButton)) {
            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            FileManager::fileManager.selectFileMode(SAVE, hWnd, penMemory); /// �߰�
        }

        /// ���� �ε�
        else if (IntersectRect(&aSide, &mouseSide, &sideLoad.rectButton)) {
            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            MessageBox(hWnd, L"file load", L"file load", MB_OK);
            FileManager::fileManager.selectFileMode(LOAD, hWnd, penMemory); /// �߰�
            SendMessage(WndFunc::toolWnd, WM_COMMAND, TL_PLAY_BT, 0); /// �߰�
        }

        /// ���� �Ŵ���
        else if (IntersectRect(&aSide, &mouseSide, &sideFM.rectButton)) {
            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            MessageBox(hWnd, L"file Manager", L"file Manager", MB_OK);
        }

        /// ũ����
        else if (IntersectRect(&aSide, &mouseSide, &sideCredit.rectButton)) {
            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            MessageBox(hWnd, L"Credit", L"Credit", MB_OK);
        }
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        /// �̹��� ��ư �̹��� ��� �� �׸��� ���� (�ʿ�� MakeButton.h(.cpp) ����)
        sideNew.drawRectButton(hdc, IDI_NEW_FILE_ICON);
        sideSave.drawRectButton(hdc, IDI_SAVE_ICON);
        sideLoad.drawRectButton(hdc, IDI_LOAD_ICON);
        sideFM.drawRectButton(hdc, IDI_FILEMANAGER_ICON);
        sideCredit.drawRectButton(hdc, IDI_CREDIT_ICON);

        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}





