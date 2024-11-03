#include "DW_FileManager.h"

HWND DW_FileManager::hListBox = nullptr; // �ʱⰪ ����

// ���� �̸��� ������ ���� �߰�
std::vector<std::wstring> fileList; // ���� ��� ����

std::wstring filePath;
std::wstring DW_FileManager::filePath; // ����

std::wstring getFilePath() {
    PWSTR desktopPath = NULL;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &desktopPath);

    if (SUCCEEDED(result)) {
        std::wstring path(desktopPath);
        CoTaskMemFree(desktopPath);  // �޸� ����
        return path + L"\\file\\";
    }
    else {
        return L"";  // ��θ� �������� ������ ��� �� ���ڿ� ��ȯ
    }
}

void DW_FileManager::saveFileList(const std::vector<std::wstring>& fileList) {
    std::wstring filePath = getFilePath() + L"\\file\\FileList.txt";  // ����ȭ�� ��ο� ����
    std::wofstream outFile(filePath);
    if (outFile.is_open()) {
        for (const auto& fileName : fileList) {
            outFile << fileName << std::endl;
        }
        outFile.close();
    }
}

std::vector<std::wstring> DW_FileManager::loadFileList() {
    std::vector<std::wstring> fileList;
    std::wstring filePath = getFilePath() + L"\\file\\FileList.txt";  // ����ȭ�� ��ο��� �ε�
    std::wifstream inFile(filePath);
    std::wstring line;

    while (std::getline(inFile, line)) {
        fileList.push_back(line);
    }
    return fileList;
}

void populateFileList(HWND hListBox) {
    // ����Ʈ�ڽ��� �ʱ�ȭ���� �������ν� ���� �׸��� ����
    std::vector<std::wstring> savedFileList = DW_FileManager::loadFileList();

    // ���� ���� ����� ����Ʈ�ڽ��� �߰�
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

    for (const auto& fileName : savedFileList) {
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)fileName.c_str());
    }

    // ���ο� ������ �˻� �� �߰�
    std::wstring filePath = getFilePath();
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW((filePath + L"*.txt").c_str(), &findFileData); // .txt ���ϸ� ã��

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // ������ ��Ͽ� �̹� �ִ��� Ȯ��
                auto it = std::find(savedFileList.begin(), savedFileList.end(), findFileData.cFileName);
                if (it == savedFileList.end()) {
                    // �ߺ��� �ƴ� ��쿡�� �߰�
                    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)findFileData.cFileName);
                    savedFileList.push_back(findFileData.cFileName);
                }
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    // ������Ʈ�� ���� ��� ����
    DW_FileManager::saveFileList(savedFileList);
}

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
        return pThis->handleMessageFM(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT DrowWindow::handleMessageFM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        RECT rect;
        GetClientRect(WndFunc::drowWnd, &rect);

        int width = rect.right - rect.left- 1400;
        int height = rect.bottom - rect.top - 570;

        DW_FileManager::hListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER,
            rect.left + 8, rect.top + 8, width, height, hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);

        SendMessage(DW_FileManager::hListBox, LB_SETHORIZONTALEXTENT, width, 0);

        ShowWindow(DW_FileManager::hListBox, SW_HIDE);

        populateFileList(DW_FileManager::hListBox);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 101 && HIWORD(wParam) == LBN_DBLCLK) {
            int selectedIndex = SendMessage(DW_FileManager::hListBox, LB_GETCURSEL, 0, 0);
            if (selectedIndex != LB_ERR) {
                wchar_t selectedFileName[256] = {};
                SendMessage(DW_FileManager::hListBox, LB_GETTEXT, selectedIndex, (LPARAM)selectedFileName);

                std::wstring selectedFile(selectedFileName);

                std::wstring desktopPath = getFilePath();

                DW_FileManager::filePath = desktopPath + selectedFile;

                DWORD fileAttr = GetFileAttributesW(DW_FileManager::filePath.c_str());
                if (fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
                    FileManager::fileManager.selectFileMode(SD_FILEMANAGER_BT, hWnd, DW_SideMenu::penMemory);
                }
                else {
                    MessageBox(hWnd, L"������ �������� �ʽ��ϴ�.", L"�˸�", MB_OK);
                }
            }
        }
        break;
    }
    case WM_PAINT: {
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