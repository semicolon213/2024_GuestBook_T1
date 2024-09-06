#pragma once

#include <Windows.h>
#include <vector>
#include <string>

#define ID_RIGHT_PANEL 102
#define ID_FILE_LIST 101
#define ID_BUTTON_SAVE 200

class FileManager
{
private:
	static FileManager& getInstance() {
		static FileManager instance;
		return instance;
	}
	 HWND hRightPanel;
	 HWND hFileListBox;
	 HWND File_Manager_Button;
	 HINSTANCE hInst;


	 
	 

public:
	FileManager() = default;
	~FileManager() = default;
	/*�г� �Լ�*/
	static void InitializePanels(HWND hWnd);
	static void ResizePanels(HWND hWnd, LPARAM lParam);

	/*��ư �Լ�*/
	static void AddFileToList(const std::wstring& fileName);
	static void Button(HWND hWnd);
	static void UpdateFileListUI();
};






