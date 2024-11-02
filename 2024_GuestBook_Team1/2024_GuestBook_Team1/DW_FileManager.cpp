#include "DW_FileManager.h"

HWND DW_FileManager::hListBox = nullptr; // �ʱⰪ ����

// ���� �̸��� ������ ���� �߰�
std::vector<std::wstring> fileList; // ���� ��� ����

std::wstring filePath;
std::wstring DW_FileManager::filePath; // ����

// ���� �Ŵ��� ��ư Ŭ�� ���θ� üũ�ϴ� ����

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

/// ���� �� ���� �޼���
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
        return pThis->handleMessageFM(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}



/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageFM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        // ����Ʈ�ڽ� ���� ����
        DW_FileManager::hListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
            0, 0, 500, 500, hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);

        // �ʱ� ���¿��� ����Ʈ�ڽ��� ����
        ShowWindow(DW_FileManager::hListBox, SW_HIDE);

        // ���� ����� ����Ʈ�ڽ��� �߰�
        populateFileList(DW_FileManager::hListBox); // .txt ���� ����� ����Ʈ�ڽ��� �߰��մϴ�.
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


        break;
    }
    case WM_COMMAND:
    {
            // ����Ʈ�ڽ� ���� Ŭ�� �޽��� ó��
        if (LOWORD(wParam) == 101 && HIWORD(wParam) == LBN_DBLCLK) {
            int selectedIndex = SendMessage(DW_FileManager::hListBox, LB_GETCURSEL, 0, 0);
            if (selectedIndex != LB_ERR) {
                // ������ �׸��� �ؽ�Ʈ ��������
                wchar_t selectedFileName[256] = {};
                SendMessage(DW_FileManager::hListBox, LB_GETTEXT, selectedIndex, (LPARAM)selectedFileName);

                // ������ ���� �̸�
                std::wstring selectedFile(selectedFileName);

                // ���� ��� ��������
                std::wstring desktopPath = getFilePath();

                // ��ü ���� ��� ����
                DW_FileManager::filePath = desktopPath + selectedFile;

                // ���� ���� ���� Ȯ��
                DWORD fileAttr = GetFileAttributesW(DW_FileManager::filePath.c_str());
                if (fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
                    FileManager::fileManager.selectFileMode(SD_FILEMANAGER_BT, hWnd, DW_SideMenu::penMemory);
                }
                else {
                    // ������ �������� ����
                    MessageBox(hWnd, L"������ �������� �ʽ��ϴ�.", L"�˸�", MB_OK);

                }
            }

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

    

