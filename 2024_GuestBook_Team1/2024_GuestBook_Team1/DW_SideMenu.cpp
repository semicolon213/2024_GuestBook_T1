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

std::vector<PINFO>* DW_SideMenu::penMemory = new std::vector<PINFO>; /// �߰�

RECT mouseSide;     /// InterSect�� ���� ���콺 ��ǥ ���� RECT
RECT aSide;         /// InterSect�� ��ȯ�Ǵ� RECT

bool isListBoxVisible = true; // �ʱⰪ: ����Ʈ�ڽ��� ���̵��� ����

/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
    {
        // ����ȭ���� "file" ���� ��� ��������
        std::wstring fileFolderPath = DW_FileManager::getFilePath();

        // "file" ���� ���� ���� Ȯ�� �� ����
        DWORD ftyp = GetFileAttributesW(fileFolderPath.c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES) {
            // ������ �������� �ʴ� ���, ����
            if (!CreateDirectoryW(fileFolderPath.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
                // ���� ���� ���� ó��
                MessageBox(hWnd, L"���� ������ ������ �� �����ϴ�.", L"����", MB_OK);
                return -1; // ���� ���� �� ������ ���� �ߴ�
            }
        }

        break;
    }
    case WM_COMMAND:
    {

        break;
    }
    case WM_LBUTTONDOWN:
    {
        if (function->getIsReplay()) {
            break;
        }
        /// ��Ŭ���� ���� ���콺 ��ǥ(lParam)�� �޾� ���콺 RECT ����
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        mouseSide.left = x;
        mouseSide.top = y;
        mouseSide.right = mouseSide.left + 1;
        mouseSide.bottom = mouseSide.top + 1;

        /// ���� ������
        if (IntersectRect(&aSide, &mouseSide, &sideNew.rectButton)) {
            function->setisLeftClick(false);

            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            /// �޼��� �ڽ��� �̹��� ��ư Ȱ��ȭ ���� �˾ƺ��� ���� �ӽ÷� �߰��� ��
            if (function->getDrawLInfoEmpty()) { break; }
            if (!function->getIsReplay()) {
                SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_CLEAR_BT, 0);
                SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)L"�̸� ����");
            }
        }

        /// ���� ���̺�
        else if (IntersectRect(&aSide, &mouseSide, &sideSave.rectButton)) {
            function->setisLeftClick(false);

            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            FileManager::fileManager.selectFileMode(SAVE, hWnd, DW_SideMenu::penMemory); /// �߰�
        }

        /// ���� �ε�
        else if (IntersectRect(&aSide, &mouseSide, &sideLoad.rectButton)) {
            function->setisLeftClick(false);


            DW_SideMenu::penMemory->clear();

            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            FileManager::fileManager.selectFileMode(LOAD, hWnd, DW_SideMenu::penMemory); /// �߰�
        }

        /// ���� �Ŵ���
        else if (IntersectRect(&aSide, &mouseSide, &sideFM.rectButton)) {
            function->setisLeftClick(false);

            // ����Ʈ�ڽ� ���̱�/����� ó��

            RECT rect;
            GetClientRect(WndFunc::drowWnd, &rect);
            int Width = rect.right;
            int Height = rect.top;
            int Fm_x = Width - 250;
            int Fm_y = Height + 110;
            int x_2 = 190;
            int y_2 = 302;
            //������ ���̸� ���̷��� createWindowFM�� 3��°(x_2) ��ǥ���� ���̱� 
            //���� ���̸� ���̷��� Fm_x ���� �� ���̸� �˴ϴ�.  ��������� ����(��) �� ��ü�� ���̰� ������ ���� ����� ���� ������ �� �� ����ؾ� ��.

            createWindowFM(Fm_x, Fm_y, x_2, y_2, WndFunc::drowWnd); //���ϸŴ��� ������ ���� 

            isListBoxVisible = !isListBoxVisible; // ���� ���� ����

            ShowWindow(DW_FileManager::hListBox, isListBoxVisible ? SW_HIDE : SW_SHOW); // ����Ʈ�ڽ� ���̱�/�����
            ShowWindow(WndFunc::fileManager, isListBoxVisible ? SW_HIDE : SW_SHOW); // ����Ʈ�ڽ� ���̱�/�����   
        }

        /// ũ����
        else if (IntersectRect(&aSide, &mouseSide, &sideCredit.rectButton)) {
            SendMessage(GetParent(WndFunc::drowWnd), WM_COMMAND, DEF_CREDIT_BT, 0);
        }
        break;
    }
    case WM_SIZE: // â ������ ���� �� ���̵�޴� ���󰡵��� ����.
    {
        RECT rect;
        GetClientRect(WndFunc::drowWnd, &rect);

        int Width = rect.right;
        int Height = rect.top;

        int Fm_x = Width - 250;
        int Fm_y = Height + 110;
        int n_Width = 190;
        int n_Height = 302;

        // ���ϸŴ��� â�� ũ��� ��ġ ������Ʈ
        MoveWindow(WndFunc::fileManager, Fm_x, Fm_y, n_Width, n_Height, TRUE);
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
