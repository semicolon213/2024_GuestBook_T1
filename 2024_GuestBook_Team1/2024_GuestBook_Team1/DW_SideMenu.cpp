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

HWND DW_SideMenu::hListBox = nullptr; // �ʱⰪ ����

// ���� �̸��� ������ ���� �߰�
std::vector<std::wstring> fileList; // ���� ��� ����

std::wstring filePath;
std::wstring DW_SideMenu::filePath; // ����

// ���� �Ŵ��� ��ư Ŭ�� ���θ� üũ�ϴ� ����
bool isListBoxVisible = true; // �ʱⰪ: ����Ʈ�ڽ��� ���̵��� ����

std::wstring getFilePath() {
    return L"C:\\2024_GuestBook_T1\\file\\"; // ���� ��η� ����
}

void populateFileList(HWND hListBox) {
    std::wstring filePath = getFilePath();
    std::wcout << L"Searching in: " << filePath << std::endl; // �߰��� �α�
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW((filePath + L"*.txt").c_str(), &findFileData); // .txt ���ϸ� ã��

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // ���� �̸��� ����Ʈ�ڽ��� �߰�
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { // ���丮 ����
                SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)findFileData.cFileName);
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}


/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
    {
        // ����Ʈ�ڽ� ���� ����
        DW_SideMenu::hListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
            0, 0, 100, 100, hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);

        // �ʱ� ���¿��� ����Ʈ�ڽ��� ����
        ShowWindow(DW_SideMenu::hListBox, SW_HIDE);

        // ���� ����� ����Ʈ�ڽ��� �߰�
        populateFileList(DW_SideMenu::hListBox); // .txt ���� ����� ����Ʈ�ڽ��� �߰��մϴ�.

        break;
    }
    case WM_COMMAND:
    {
        // ����Ʈ�ڽ� ���� Ŭ�� �޽��� ó��
        if (LOWORD(wParam) == 101 && HIWORD(wParam) == LBN_DBLCLK) {
            int selectedIndex = SendMessage(DW_SideMenu::hListBox, LB_GETCURSEL, 0, 0);
            if (selectedIndex != LB_ERR) {
                // ������ �׸��� �ؽ�Ʈ ��������
                wchar_t selectedFileName[256] = {};
                SendMessage(DW_SideMenu::hListBox, LB_GETTEXT, selectedIndex, (LPARAM)selectedFileName);

                // ������ ���� �̸�
                std::wstring selectedFile(selectedFileName);

                // ���� ��� ��������
                std::wstring desktopPath = getFilePath();

                // ��ü ���� ��� ����
                DW_SideMenu::filePath = desktopPath + selectedFile;

                // ���� ���� ���� Ȯ��
                DWORD fileAttr = GetFileAttributesW(DW_SideMenu::filePath.c_str());
                if (fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
                    FileManager::fileManager.selectFileMode(SD_FILEMANAGER_BT, hWnd, penMemory); /// �߰�
                }
                else {
                    // ������ �������� ����
                    MessageBox(hWnd, L"������ �������� �ʽ��ϴ�.", L"�˸�", MB_OK);

                }
            }
            break;
        }
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
            if (function->getDrawLInfoEmpty()) { break; }
            if (!function->getIsReplay()) {
                SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_CLEAR_BT, 0);
                SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)L"�̸� ����"); /// 2024_GuestBook_Team1�� �޽��� ���� /// �߰�
            }
        }

        /// ���� ���̺�
        else if (IntersectRect(&aSide, &mouseSide, &sideSave.rectButton)) {
            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            FileManager::fileManager.selectFileMode(SAVE, hWnd, penMemory); /// �߰�
        }

        /// ���� �ε�
        else if (IntersectRect(&aSide, &mouseSide, &sideLoad.rectButton)) {
            /// �޼��� �ڽ� ���ְ� if��(InTerSect)���ο� ���� �ڵ� �߰�
            FileManager::fileManager.selectFileMode(LOAD, hWnd, penMemory); /// �߰�
            SendMessage(WndFunc::toolWnd, WM_COMMAND, TL_PLAY_BT, 0); /// �߰�
        }

        /// ���� �Ŵ���
        else if (IntersectRect(&aSide, &mouseSide, &sideFM.rectButton)) {
            // ����Ʈ�ڽ� ���̱�/����� ó��
            isListBoxVisible = !isListBoxVisible; // ���� ���� ����

            ShowWindow(DW_SideMenu::hListBox, isListBoxVisible ? SW_HIDE : SW_SHOW); // ����Ʈ�ڽ� ���̱�/�����
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
}
