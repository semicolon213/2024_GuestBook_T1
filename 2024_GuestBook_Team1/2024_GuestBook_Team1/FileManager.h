#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Function.h"
#include "Struct.h"
#include "Resource.h"
#include "Function.h"
#include "DW_SideMenu.h"
#include "DW_NameBar.h"
#include "WndFunc.h"
#include <unordered_set> //���ϸ���Ʈ �ߺ� ���� ���  
#include <shlobj.h> //���̺����� ���� ��� Ȯ�� 
#include "DW_FileManager.h"


class DW_SideMenu;

class FileManager
{
private:

    bool HandleFileOperation(HWND hWnd, std::vector<PINFO>* penMemory, bool isSave);
    bool ConfigureDialog(HWND hWnd, DWORD flags, WCHAR* fileBuffer, DWORD bufferSize);
    bool save(const wchar_t* path, std::vector<PINFO>* penMemory, HWND hWnd);
    bool load(const wchar_t* path, std::vector<PINFO>* penMemory, HWND hWnd);
    bool openForWrite(const wchar_t* path);
    bool openForRead(const wchar_t* path);

    OPENFILENAME OFN = { 0 };
    WCHAR fileName[260] = { 0 };
    WCHAR fileOpenName[260] = { 0 };
    WCHAR str[260] = { 0 };

    std::fstream fs;
    HWND m_hWnd;
    bool isPanelVisible;

public:
    FileManager() = default;
    ~FileManager() = default;

    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

    void selectFileMode(int wmId, HWND cWnd, std::vector<PINFO>* penMemory);
    static FileManager fileManager;

    void AddFileToList(const std::wstring& fileName);
    void UpdateFileListUI();

    std::vector<std::wstring> savedFileList;  // ���ϸ��� �����ϴ� ����Ʈ
    HWND hFileListBox;  /// ���� ����� ǥ���� ����Ʈ �ڽ�
    HWND hRightPanel;   /// �г� �ڵ�
    HINSTANCE hInst;    /// �ν��Ͻ� �ڵ�

    void SaveFileList(); ///���ϸ���Ʈ ����
    void LoadFileList(); ///���ϸ���Ʈ �ҷ�����

    static std::wstring baseName; // ���� ���� ����

    std::unique_ptr<Function> function;
};
